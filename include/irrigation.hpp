#pragma once
#include <cstdint>
#include <stdexcept>
enum class IrrigationState { Cooldown, Ready, Watering, Fault };
struct IrrigationConfig {
  int dry = 2600, wet = 1900;
  std::uint32_t maxRun = 12000, cooldown = 300000;
  unsigned confirmations = 5;
};
struct IrrigationStatus {
  IrrigationState state;
  bool pump;
  int moisture;
  std::uint32_t sequence;
  const char *reason;
};
class IrrigationController {
  IrrigationConfig config_;
  IrrigationState state_ = IrrigationState::Cooldown;
  std::uint32_t changed_ = 0, sequence_ = 0, lastSample_ = 0;
  unsigned dryCount_ = 0;
  int filtered_ = 0;
  bool initialized_ = false;
  const char *reason_ = "boot_lockout";
  void change(IrrigationState state, std::uint32_t now, const char *reason) {
    if (state_ != state) {
      state_ = state;
      changed_ = now;
      sequence_++;
    }
    reason_ = reason;
  }

public:
  explicit IrrigationController(IrrigationConfig c = {}) : config_(c) {
    if (c.wet < 1 || c.dry >= 4095 || c.wet >= c.dry || !c.confirmations ||
        !c.maxRun || c.maxRun >= 0x80000000U || !c.cooldown ||
        c.cooldown >= 0x80000000U)
      throw std::invalid_argument("invalid irrigation configuration");
  }
  IrrigationStatus sample(int raw, bool reservoir, bool manual,
                          std::uint32_t now) {
    if (raw <= 0 || raw >= 4095 || !reservoir ||
        (initialized_ && std::uint32_t(now - lastSample_) > 5000)) {
      change(IrrigationState::Fault, now,
             !reservoir ? "reservoir_empty" : "sensor_fault");
      dryCount_ = 0;
      return snapshot();
    }
    filtered_ = initialized_ ? (filtered_ * 3 + raw) / 4 : raw;
    initialized_ = true;
    lastSample_ = now;
    if (state_ == IrrigationState::Fault) {
      if (manual) {
        dryCount_ = 0;
        change(IrrigationState::Cooldown, now, "fault_acknowledged");
      }
      return snapshot();
    }
    if (state_ == IrrigationState::Watering) {
      if (filtered_ <= config_.wet ||
          std::uint32_t(now - changed_) >= config_.maxRun) {
        change(IrrigationState::Cooldown, now,
               filtered_ <= config_.wet ? "target_reached" : "run_limit");
        dryCount_ = 0;
      }
      return snapshot();
    }
    if (state_ == IrrigationState::Cooldown &&
        std::uint32_t(now - changed_) >= config_.cooldown)
      change(IrrigationState::Ready, now, "lockout_complete");
    dryCount_ =
        filtered_ >= config_.dry
            ? (dryCount_ < config_.confirmations ? dryCount_ + 1 : dryCount_)
            : 0;
    if (state_ == IrrigationState::Ready &&
        (manual || dryCount_ >= config_.confirmations))
      change(IrrigationState::Watering, now,
             manual ? "manual_start" : "dry_soil");
    return snapshot();
  }
  IrrigationStatus snapshot() const {
    return {state_, state_ == IrrigationState::Watering, filtered_, sequence_,
            reason_};
  }
};
