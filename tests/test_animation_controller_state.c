// Behaviour test for CAnimationController's compatibility state.
//
// This deliberately uses GetProcAddress from C: it exercises the Windows x64
// call ABI and MSVC export names without asking MinGW to emit C++ mangling.
#include <windows.h>
#include <stdio.h>
#include <string.h>

static int failures = 0;

#define CHECK(condition, message) do {                                      \
    if (condition) {                                                        \
        printf("PASS: %s\n", message);                                     \
    } else {                                                               \
        printf("FAIL: %s\n", message);                                     \
        ++failures;                                                        \
    }                                                                      \
} while (0)

#define LOAD(name, type, symbol)                                            \
    type name = (type)GetProcAddress(module, symbol);                       \
    if (!(name)) {                                                         \
        printf("missing export: %s\n", symbol);                            \
        return 2;                                                          \
    }

typedef void* (WINAPI *AddAnimationObjectFn)(void*, void*);
typedef void* (WINAPI *FindGroupByIdFn)(void*, unsigned int);
typedef void* (WINAPI *FindGroupByStoryboardFn)(void*, void*);
typedef void  (WINAPI *AddToStoryboardFn)(void*, void*, int);
typedef void  (WINAPI *ValueChangedFn)(void*, void*, void*, void*, double, double);
typedef int   (WINAPI *FindObjectFn)(void*, void*, void**, void**);
typedef int   (WINAPI *AnimateGroupFn)(void*, unsigned int, int);
typedef int   (WINAPI *ScheduleGroupFn)(void*, unsigned int, double);
typedef int   (WINAPI *IsInProgressFn)(void*);
typedef void  (WINAPI *UpdateManagerFn)(void*);
typedef void* (WINAPI *GetServiceFn)(void*);
typedef void* (WINAPI *GetStaticKeyframeFn)(void);
typedef void  (WINAPI *RemoveAllFn)(void*);

int main(void) {
    HMODULE module = LoadLibraryA("openmfc.dll");
    if (!module) {
        printf("LoadLibrary(openmfc.dll) failed: %lu\n", GetLastError());
        return 2;
    }

    LOAD(addObject, AddAnimationObjectFn,
         "?AddAnimationObject@CAnimationController@@QEAAPEAVCAnimationGroup@@PEAVCAnimationBaseObject@@@Z");
    LOAD(findGroupById, FindGroupByIdFn,
         "?FindAnimationGroup@CAnimationController@@QEAAPEAVCAnimationGroup@@I@Z");
    LOAD(findGroupByStoryboard, FindGroupByStoryboardFn,
         "?FindAnimationGroup@CAnimationController@@QEAAPEAVCAnimationGroup@@PEAUIUIAnimationStoryboard@@@Z");
    LOAD(addTransitions, AddToStoryboardFn,
         "?AddTransitions@CAnimationGroup@@IEAAXPEAUIUIAnimationStoryboard@@H@Z");
    LOAD(onValueChanged, ValueChangedFn,
         "?OnAnimationValueChanged@CAnimationController@@UEAAXPEAVCAnimationGroup@@PEAVCAnimationBaseObject@@PEAUIUIAnimationVariable@@NN@Z");
    LOAD(findObject, FindObjectFn,
         "?FindAnimationObject@CAnimationController@@QEAAHPEAUIUIAnimationVariable@@PEAPEAVCAnimationBaseObject@@PEAPEAVCAnimationGroup@@@Z");
    LOAD(animateGroup, AnimateGroupFn,
         "?AnimateGroup@CAnimationController@@QEAAHIH@Z");
    LOAD(scheduleGroup, ScheduleGroupFn,
         "?ScheduleGroup@CAnimationController@@QEAAHIN@Z");
    LOAD(isInProgress, IsInProgressFn,
         "?IsAnimationInProgress@CAnimationController@@UEAAHXZ");
    LOAD(updateManager, UpdateManagerFn,
         "?UpdateAnimationManager@CAnimationController@@UEAAXXZ");
    LOAD(getManager, GetServiceFn,
         "?GetUIAnimationManager@CAnimationController@@QEAAPEAUIUIAnimationManager@@XZ");
    LOAD(getTimer, GetServiceFn,
         "?GetUIAnimationTimer@CAnimationController@@QEAAPEAUIUIAnimationTimer@@XZ");
    LOAD(getFactory, GetServiceFn,
         "?GetUITransitionFactory@CAnimationController@@QEAAPEAUIUIAnimationTransitionFactory@@XZ");
    LOAD(getLibrary, GetServiceFn,
         "?GetUITransitionLibrary@CAnimationController@@QEAAPEAUIUIAnimationTransitionLibrary@@XZ");
    LOAD(getStartKeyframe, GetStaticKeyframeFn,
         "?GetKeyframeStoryboardStart@CAnimationController@@SAPEAVCBaseKeyFrame@@XZ");
    LOAD(removeAll, RemoveAllFn,
         "?RemoveAllAnimationGroups@CAnimationController@@QEAAXXZ");

    unsigned char controller[112];
    unsigned char animationObject[64];
    unsigned char animationVariable[64];
    unsigned char storyboard[64];
    memset(controller, 0, sizeof(controller));
    memset(animationObject, 0, sizeof(animationObject));
    memset(animationVariable, 0, sizeof(animationVariable));
    memset(storyboard, 0, sizeof(storyboard));

    void* group = addObject(controller, animationObject);
    CHECK(group != NULL, "AddAnimationObject creates a group");

    unsigned int groupId = 0;
    for (unsigned int candidate = 1; candidate < 1024; ++candidate) {
        if (findGroupById(controller, candidate) == group) {
            groupId = candidate;
            break;
        }
    }
    CHECK(groupId != 0, "group is discoverable by its allocated id");

    addTransitions(group, storyboard, TRUE);
    CHECK(findGroupByStoryboard(controller, storyboard) == group,
          "storyboard lookup returns its registered group");

    void* foundObject = (void*)1;
    void* foundGroup = (void*)1;
    CHECK(findObject(controller, animationVariable, &foundObject, &foundGroup) == FALSE,
          "unknown variable is not mapped to an arbitrary object");
    CHECK(foundObject == NULL && foundGroup == NULL,
          "failed variable lookup clears both outputs");

    onValueChanged(controller, group, animationObject, animationVariable, 1.0, 2.0);
    CHECK(findObject(controller, animationVariable, &foundObject, &foundGroup) == TRUE,
          "value callback registers the variable association");
    CHECK(foundObject == animationObject && foundGroup == group,
          "variable lookup returns the matching object and group");

    CHECK(isInProgress(controller) == FALSE, "controller starts idle");
    CHECK(animateGroup(controller, groupId, TRUE) == TRUE,
          "AnimateGroup accepts a registered group");
    CHECK(isInProgress(controller) == TRUE, "AnimateGroup marks work active");
    updateManager(controller);
    CHECK(isInProgress(controller) == FALSE, "manager update completes active work");

    CHECK(scheduleGroup(controller, groupId, 1.0) == TRUE,
          "ScheduleGroup accepts a registered group");
    CHECK(isInProgress(controller) == TRUE, "scheduled work reports in progress");
    updateManager(controller);
    CHECK(isInProgress(controller) == FALSE, "manager update completes scheduled work");

    CHECK(getManager(controller) == NULL && getTimer(controller) == NULL &&
          getFactory(controller) == NULL && getLibrary(controller) == NULL,
          "unavailable UIAnimation services return null, not fake interfaces");
    CHECK(getStartKeyframe() == NULL,
          "unavailable storyboard-start keyframe returns null");

    removeAll(controller);
    CHECK(findGroupById(controller, groupId) == NULL,
          "RemoveAllAnimationGroups clears the registry");

    FreeLibrary(module);
    printf("animation controller checks: %d failure(s)\n", failures);
    return failures == 0 ? 0 : 1;
}
