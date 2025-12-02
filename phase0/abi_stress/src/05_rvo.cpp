#include "05_rvo.h"

Pod8 CStage5_RVO::RetPod8() { return {1, 2}; }
Pod16 CStage5_RVO::RetPod16() { return {1, 2}; }
Pod24 CStage5_RVO::RetPod24() { return {1, 2, 3}; }
NonPod8 CStage5_RVO::RetNonPod8() { return {1, 2}; }
