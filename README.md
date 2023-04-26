A basic animation repo. This was done on Mac with an old version of openGL, so lots of bad drawing code (calling glBegin(GL_TRIANGLES) instead of using buffers made me cry)

This has some cool stuff in it. Loading skeletons, skins, and keyframed animations to start, a cloth sim (that blows up most of the time, but at a slow enough update delta stays together), and quaternion rotation code for smooth interpolation.

This repo does build, and is currently set up to run the quaternion interpolation code.
