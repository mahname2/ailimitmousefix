# Overview

Mouse sensitivity in AI Limit is proportional to frametime. This causes several issues:

 - When playing at high framerates, mouse sensitivity becomes unreasonably low, making high framerates unplayable.
 - Inconsistent framerate leads to inconsistent sensitivity.
 - As a special case of the previous point, stutters cause the sensitivity to spike. This effectively makes the camera rotate significantly whenever the game stutters, which often is deadly (especially during parkour segments).


This mod decouples mouse sensitivity from frametime, solving the aforementioned issues. After installing, mouse sensitivity will always act as if the framerate is 50 (but the camera still updates every frame, of course).

# Compilation

Compile using the compiler of your choice, e.g.

```
	gcc main.c -o AI_LIMIT_Mouse_Sensitivity_Fix_Patcher.exe
```

# Usage

 1. Compile or download the patcher executable.
 2. Move the executable to the game installation folder (placing it in the same folder as to AI-LIMIT.exe and GameAssembly.dll).
 3. Run the executable and wait for it to finish.
 4. (Optional) Delete the patcher executable.
 5. Run the game.

# Uninstallation

 1. Delete GameAssembly.dll.
 2. Rename GameAssembly.dll.bak to GameAssembly.dll.


# How the mod works

When updating camera position in CameraManager$$CameraFollow(), the game makes a call to Utility$UpdateLerpFix(). Utility$UpdateLerpFix() simply multiples the measured mouse movement by 50\*Time.deltaTime, resulting in the frametime dependent behaviour seen (Time.deltaTime is Unity's measured frametime).

The demo of the game only updated the camera 50 times a second, before being changed to updating every frame in the full release. Utility$UpdateLerpFix is likely intended to compensate for the variable number of updates when rotation is updated every frame instead, which is appropriate for some input devices. However, mouse movement distance is already frametime dependent (if the frame is shorter, then your mouse won't have time to move as far), so this intended correction is not necessary (and instead makes things worse).

This mod replaces the call to Utility$UpdateLerpFix with a NOP instruction (that is, an instruction that does nothing).