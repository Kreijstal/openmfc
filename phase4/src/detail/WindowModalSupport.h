#pragma once

namespace openmfc::detail {

inline constexpr unsigned int wfModalLoop = 0x08;
inline constexpr unsigned int wfContinueModal = 0x10;
inline constexpr unsigned long mlfNoIdleMessage = 0x01;
inline constexpr unsigned long mlfNoKickIdle = 0x02;
inline constexpr unsigned long mlfShowOnIdle = 0x04;
inline constexpr unsigned int wmKickIdle = 0x036a;

}
