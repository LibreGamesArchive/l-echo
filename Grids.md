Grids are the platforms.  They are implemented as linked-lists/webs that change depending on the angle.  Of course, this makes "perspective holes" and "perspective jumps" harder, but that's another page.

Implemented:

  * grid          -Normal grids

  * escgrid       -Grids that change position and neighbors depending on the angle of the camera.  "esc"'s are the alter egos of the "escgrid"'s, and they can be any grid pointer, with angles as keys.  Oh, and they stand for Escher-grid, but "esc" is just a abbreviation.

  * t\_grid        -Normal grids, except they link with 3 neighbors instead of the normal two.  "grid"'s don't have variable neighbors since there can only be a max of 4 anyways (well, unless you can convince me that there is a clever scenario with 5 neighbors).

  * hole          -See HolesLauncherStaticGridAndIsectGrid

  * stair         -A grid that takes in vectors for its width and direction, basically can be used as anything, like obstacles.

To be implemented:

  * launcher      -See HolesLauncherStaticGridAndIsectGrid

  * x\_grid        -Same as t\_grid but has 4 neighbors

Wishlist:

  * dsided\_grid   -Double-sided grid, like in Super Mario Galaxy.  In this case, you would fall onto the other side of the grid with a hole.