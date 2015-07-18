The last month I had been thinking of hole and launcher design.  Finally, I came to the conclusion that simple is best.  So I came up with the new, and hopefully final hole and launcher design.

The premise is simple: have the character enter another mode.  When he's walking around, he leaves the job of finding out his next position to the grids.  So when he's flying through the air, he should handle the flying himself, and that's exactly what he'll do.

Here's a sequence of events:
  1. Character (echo\_char) steps on a launcher/hole
  1. Character enters falling mode if he stepped on a hole or flying mode if he stepped on a launcher (shifts to falling mode after he reached the apex)
  1. Character controls his motion while he's flying/falling.  Here, he can mimic real world physics with an accelerated free-fall
  1. During falling mode, he checks if any line from the camera (position is calculated from the camera angle and depth) to his position crosses a grid he can land on.  If so, he "teleports" to the nearest grid immediately
  1. Character lands (landing animation), and shifts back to grid mode.

It's less math, possibly a lower overhead, and definitely simpler.