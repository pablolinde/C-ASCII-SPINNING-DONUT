# C-ASCII-SPINNING-DONUT

A terminal-based spinning donut animation inspired by the mathematical breakdown in [a1k0n's "Donut Math" article](https://www.a1k0n.net/2011/07/20/donut-math.html). This implementation adapts the article's pseudo-code to C with several customizations. The code does real-time 3D rendering using ASCII characters, implementig luminance shading with 12-level ASCII palette: .,-~:;=!*#$@. Future plans include morphing the donut shape into a letter 'C', a self-referential program written in C.

## Key customizations:

- Screen dimensions: adjustable display parameters. For optimal results, match the donut size to your terminal dimensions to balance performance and visual quality.

- K1 calculation: Uses screen height for vertical scaling (K1 = screen_height*K2*3/(8*(R1+R2))) while most implementations use screen width, this vertical approach yielded better results in my case. Adjustable via code modification.

- Aspect ratio: 2.4 correction factor compensates for terminal character proportions, preventing vertical flattening caused by rectangular ASCII characters.

- Rotation speeds: A += 0.04 (vertical rotation) and B += 0.08 (horizontal rotation). Both values can be freely modified to adjust spinning dynamics.

- Animation timing: usleep(50000) delivers approximately 20 FPS.

## Dependencies:

- C compiler (GCC recommended)
- Unix-like terminal (for ANSI escape codes)

## Compilation and usage:

Open terminal, go to the directory where donut.c is located and run: gcc donut.c -lm -o donut && ./donut

Stop the animation with Ctrl+C.
