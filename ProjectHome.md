Notice: As of 3-20-09, I am no longer developing this.  This is a failure on my part, because of my bad coding practices.  I can no longer maintain and add to my code, so this project is becoming a huge time sink.  I'm sorry for those who wanted a full echochrome clone; I guess you'll have to wait for a good PSP emulator, or another programmer who is willing to write another echochrome clone (I'm happy to teach the concepts to anyone who wants to take up the challenge).

Update 7-30-09, it occurred to me (in the process of preparing the last n-echo release for a homebrew contest) that I never finished the doc on how to create stages.  That's finally fixed.


---


L-Echo is a free and open source clone of the SCEJ game Echochrome (Mugen Kairou in Japan) (PSP/PS3).   It was written for Linux but has been ported to Windows, Mac OS X (10.4+) **(NOT PPC64)** and the Nintendo DS (see right below).

[L-Echo Video (0.4.1)](http://www.youtube.com/watch?v=-9M15RBDUDY)

N-Echo is a port (in the same source tree) for the Nintendo DS.

[N-Echo Video (0.4.0)](http://www.youtube.com/watch?v=Rjv6s7molRI)

**IF THERE ARE ANY ISSUES, JUST PLEASE, TAKE A SECOND, AND FILE AN ISSUE, using the "Issues" tab on the top there!**

**Releases**:

**0.4.3** - Holy crap:

![http://img7.imageshack.us/img7/6051/dashboarddq0.png](http://img7.imageshack.us/img7/6051/dashboarddq0.png)

Damn, [Free Gamer](http://freegamer.blogspot.com/) sure is popular.

Well, 0.4.3 is out, with minor changes:
  * Whopping one more stage - LEcho1.xml is an exclusive puzzle (I think) I made; **IT ONLY WORKS WITH 0.4.3+** (a bug in earlier versions crashes the game after you get to the first echo).  I realized that the zoom level is horrible, so I promise I'll put a zoom function into the next release.
  * Falling Animation - It isn't as good as I would like it to be, but I don't think I can expect to much from trig functions.


---


**0.4.2** - Finally, after 30 revisions and nearly 2 months, here's release 0.4.2.  I didn't use 0.5 because the 0.4.x series (animations) is still not done yet.  Consider 0.4.2 as a large maintenance release.  Changes:
  * NEWNewHolesAndLaunchers - Complete clone of echochrome and OLE Coordinate System.  Yes, including launchers.
  * FPS - 30Hz across all platforms
  * Spawning - The mannequin now spawns at the last echo.
  * Documentation - Most of the code is now well-documented.
After 0.4.x, 0.5.x will consist of the level creator, which will be Java-based.  Audio will be implemented in 0.6.x.  It will play ONLY WAV/PCMs.

[revision 103](https://code.google.com/p/l-echo/source/detail?r=103) - [r103](https://code.google.com/p/l-echo/source/detail?r=103) pretty much completes the documentation project.  Now I just need to complete the launchers, and fix the holes (I hear that the character is supposed to rotate in space too...?).

[revision 96](https://code.google.com/p/l-echo/source/detail?r=96) - Still...trucking...on...to...the...next...release...
N-Echo is working correctly now, and, with the new libnds, it's likely to stream WAV instead of MP3.

[revision 83](https://code.google.com/p/l-echo/source/detail?r=83) - By no means is [r83](https://code.google.com/p/l-echo/source/detail?r=83) working correctly, but here are some changes you'll see in 0.4.2. The release has deviated from the RoadMap, but it's a needed maintenance:
  * NEWNewHolesAndLaunchers - The new implementation of holes and launchers is WAY simpler than the other 2 designs.  Holes is working as of [r83](https://code.google.com/p/l-echo/source/detail?r=83), though there are some strange bugs, like the character falling into the grid next to it.  I'll have to add some distance restrictions.  Launchers still need to be implemented, but it should be pretty simple, and similar to holes, though I still need to know just how high the mannequin jumps.
  * Audio support - You can't play it yet (probably will read from a playlist in a prefs file), but the functions have been put in place (see echo\_mp3.h and echo\_mp3.cpp) OpenAL for Windows/Mac/Linux, and [ASLib](http://nodadev.wordpress.com/nds-projects/advanced-sound-library/) for NDS.  MP3s will be used for music, though I'm still having trouble with sound effects such as the mannequin's footsteps (probably WAVs for OpenAL; you can do weird shit like Doppler effect in there).
  * FPS - Has been fixed to 30Hz
  * Spawning - The mannequin now spawns at the last echo.
  * Position - Technical stuff: the position in grid\_info\_t is now a pointer for consistency.
  * Documentation - I'm starting to document the source code via Doxygen, mainly to attract new developers (perhaps after 1.0, or now).

0.4.1 - Through the magic that is [Inverse Kinematics](http://en.wikipedia.org/wiki/Inverse_kinematic_animation), the mannequin can walk up stairs now!  Yay!

0.4.0 - THANK GOD FOR PUGIXML!  N-Echo is BACK with WALKING ANIMATION!

To make this change, I had to abstract the xml libraries away; just by changing the Makefile I can make l-echo compile with TinyXML (works with l-echo, but not n-echo), rapidxml (buggy) or pugxml (windows only).  Just look at echo\_xml.h and echo\_xml.cpp.

You'll probably also notice that my coding style has changed.  Now new code and revised code will by status-based.

You'll probably also notice that the mannequin in n-echo is unnaturally fast...I'll fix the FPS next release, because l-echo runs at 30 fps and n-echo runs at 60...Oops.

[revision 66](https://code.google.com/p/l-echo/source/detail?r=66)/67 - Not so much as a release, but since I don't want to give anyone the idea that this project is dead; it's far from it, because I'm not dedicating pretty much all my time into the project.

Anyways, some changes:

1) WALK ANIMATION!!!!!SHIFT+1111!!!1!1!  It's not too realistic or like the mannequin from Echochrome, but it's a start.

2) The build system now includes the template itself, because I probably broke GPL in the time I took down the nds template.  I apologize to all who wanted to compile n-echo.  The instructions to compile from SVN has also been updated.

3) Speaking of SVN, I've just realized that [revision 60](https://code.google.com/p/l-echo/source/detail?r=60)/61+ doesn't work for the NDS.  I again apologize, but it's because I didn't have my did to try it out.  The bug was mainly caused by he amount of vertexes the model required...So [revision 66](https://code.google.com/p/l-echo/source/detail?r=66)/67 just displays the character's head right now (for the NDS).

0.3.3 - I've slightly changed the method I use to compile for Mac, and it might work this time around if it didn't the first time...I have no idea if anyone with OS X has this working, so I'm just taking a crap shoot.

Oh, and this game **DOESN'T WORK IN MAC OS X PPC64**.  The 10.4u OpenGL/GLUT framework isn't compiled for ppc64, and I can't get the latest damn XCode dmg file open.

Constant velocity and running are added (which reminds me to update the "menu" next release).  I also apparently forgot to add running to the DS version...Next release?  I'm running out of buttons, though, so I probably have to come up with a preferences system to remember which hand you are.

0.3.1 - forgot to change the goals to be one of those shadow mannequins, started packing in reverse-engineered stages (oh, and I added the second solution to A1)

0.3.0 - Character model finished.  I should probably fix the graininess of the model (likely caused by the model's form as strips of quads. instead of a seamless polyhedron).

0.2.8 - After a hiatus with the nds version (size 16 font is not used now because the nds doesn't like tile overflow), this project is back on track, with 0.2.8 now packing and uploading as I speak.

The code has been improved overall, and the loader has been made to ignore errors and keep running rather than just quit.

The next release should be 0.3.0 with the character model, most likely hardcoded (since a .3ds or .obj would be overkill).

0.2.7 - Hardware-based outlining for all versions, because of the character (mannequin).  echochrome is done with that distinctive pen+ink style, so I should follow suit.  However, the NDS version is having some difficulties (you can see it in the stairs in A1, which is kind of hacked together).  I've posted a thread over a gbadev; waiting for answers.

0.2.6 - FIXED NDS VERSION!  I didn't catch the seg fault bugs in 0.2.4 because I didn't test it with my NDS, just with DeSmume.  Well, it isn't really fixed, there's some tile problems with the loader + info screen, but otherwise there's no real problem...I think.

Also, I've written the RoadMap for this project in the foreseeable future.

0.2.5 - As I'm writing this right now, release 0.2.5 should be uploaded to the download section.  Right now, I'm developing in a place with slower Internet connection, so uploading releases is now a laborious task that takes hours instead of minutes.

This release brings you a seperation between stairs and freeform\_grids; the former is for elevation changes, and the latter for visual obstacles, for perspective absence and existence.

0.2.4, another of those "screw-it" releases, adds a rudimentary menu so that it doesn't have to find "sample1.xml" at start time.

0.2.3 - N-Echo has a loader now, and the Windows in-game loader is fixed.  Again.  Sorry, but I didn't know that fstat doesn't work on Windows.

0.2.2 redux - L-Echo ported to Mac OS X (10.4+)!

0.2.2 - FIXED WINDOWS VERSION!!!! Lots of changes:

  * (Kind of) Scalable - As in, if you have a really really large stage, the camera will scale with it.  The text won't, though, which is what I'm going to fix sometime in the future.

  * Touch Screen/Mouse Support - BOOYAH!  It might be a little buggy though.

  * Mature Trigger System - Look in the sample for N-Echo

  * Better Holes and Launchers - no perspective jumping/falling is working like it's supposed to...well, there are still a few parts to iron out.  Try perspective\_jumping.xml to find out.  Instead of making the grids connect, try shifting the camera down from the connection, and see what happens.

Bug Fixes:

  * The stage files now work for all three platforms (N-Echo's glRotate3f is flipped, which is why the angles were negative and the controls were wacky)

  * As said before, the Windows version is now fixed, because of a stupid miscalculation (usleep is in microseconds, and Sleep is in milliseconds)

0.1.1 redux - N-ECHO LIVES!  0.1.1 has been successfully ported to the Nintendo DS!

Well, fine, I haven't learned to draw strings yet, which is why there's no key for the loader, but it works!

[N-Echo Video](http://www.youtube.com/watch?v=iNyvTIHP_AY)


0.1.1 - More of a maintenance release:
-Robust loader
-Awesome memory management

0.1.0 - In-game loaders! Yay!

Oh and a note: After much deliberation, I've decided NOT TO INCLUDE REAL ECHOCHROME LEVELS IN ANY OF THE RELEASES (for now...?)!

0.0.9 - Launchers implemented!!!

No movement on N-Echo, though...

0.0.8 - Character class and angle ranges (see perspective\_absence.xml, the other files, especially perspective\_existence.xml, don't use it yet).  Makefile is finally more streamlined!

N-Echo port is fixed up a little, but still doesn't work:

http://l-echo.googlecode.com/files/n-echo_attempt-3.zip

0.0.7 - Better hole implementation (see HolesLauncherStaticGridAndIsectGrid), Linux and Windows binaries released.  Oh, and if anyone is interested in port L-Echo to the NDS (as N-Echo?), I've started on the port here:

http://l-echo.googlecode.com/files/n-echo_attempt-2.zip

It's not function yet, though...

0.0.6 - No draw capabilities = perspective existence! (2 escgrid + 1 nodraw grid)

0.0.5 - Screw it.

0.0.4 - Stage loading, character states, and holes + "t\_grid"'s (grids with 3 neighbors...grids don't all have variable neighbors, because the max is 4...I haven't had the need to create an "x\_grid" yet, but I will sometime in the future)


---


echochrome is a trademark of Sony Computer Entertainment Inc. © 2008 Sony Computer Entertainment Inc.

"PlayStation", "PLAYSTATION" and "PS" Family logo are registered trademarks of Sony Computer Entertainment Inc.