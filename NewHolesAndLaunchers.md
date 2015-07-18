Deficiencies of the previous method:
  * Depends on every level
  * Some runtime overhead
  * Awkward transition to the correct grid to fall on

This new method will use:
  * A combination of static\_grid and escgrid (new isect\_grid?)
  * This combination will be used at regular intervals
  * The correct grids will be calculated as we fall through per grid and added as escgrids (very unelegant and possibly incompatible with new types of grids later on)

Benefits:
  * The new static/esc grid and its previous will move to an extremely short distance to its next target while keeping their relative positions, thus removing the strange transition.
  * Less math?