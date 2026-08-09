#pragma once

namespace openmfc { namespace detail { namespace animationcontroller {

// CAnimationGroup learns the storyboard while it is adding transitions and
// keyframes. Keep the controller's lookup registry in CAnimationController.cpp
// and expose only the narrow cross-class registration hook here.
void RegisterStoryboard(void* animationGroup, void* storyboard);

} } }
