# The Problem #

Implement holes and launchers.

As easy as that sounds, if you follow the conventional method, there are two roadblocks:
1) If you want to have physics checking with collision and stuff, fuhgeddaboutit.  echo\_ns, the body responsible for the character (yeah, I may need to move all that code to a seperate class), isn't implemented like that.  Besides, it would cause a O(n) iteration thru all the grids.
2) In the real game, the character doesn't move with the world when he/she's in the air.  Oh wait...I guess that would be an advantage.  But reason #1 is good enough for me to try other options.


# My Implementation #

My way of going around this is by introducing static\_grids and isect\_grids.  Let's start with static\_grid, which is the parent class of isect\_grid:

static\_grid is a grid that remains in the same place on the screen no matter what.  Remember, almost all methods in grid give the current angle, so all static\_grid has to do is to rotate the "normal position" (when the camera is at [0, 0, 0]) with the camera angle.

isect\_grid extends that.  When a hole::get\_next is called, it will get all the "levels" below it in the form of a map (float to a set of grid pointers).  This map is registered at load time.  Then it will construct a linked list of isect\_grids for each level, returning the first one, the one nearest to it.  And here's where the magic happens.

What the isect\_grid does, is that it knows the grids in each level (levels of escs are registered under the root escgrid).  When isect\_grid::get\_next is called, it will look at the finally position of itself (remember, it is a descendant of static\_grid, it will change its position based on the camera angle to make itself STAY AT THE SAME PLACE, and thus the character won't seem like it is changing its position), calculate a ray from the previous grid to itself, calculate where the ray hits the current level, and see if the endpoint is in any of the grids on the current level.  The first grid that has the point is returned as the next, or it will go on to the next isect\_grid, or return "echo\_ns::hole\_grid", the pointer that denotes that the character has died.

The base position of the isect\_grid is pretty much the only diffs between holes and launchers.  Holes would just have the isect\_grids start right below it, while launchers would have a constant parabola in the direction it faces.

That's my thoughts.  Any other implementations? Comment, or better yet, implement it yourself!  [Revision 15](https://code.google.com/p/l-echo/source/detail?r=15) and on (versions 0.0.7+) have static\_grids and isect\_grids.