#include <stdio.h>
#include <math.h>
#include <unistd.h>

// Window where the donut will be rendered, 
// for optimal results, match the donut size to your terminal dimensions to balance performance and visual quality.
#define screen_width 64 
#define screen_height 28
#define pi 3.14159265358979323846

// Angular steps for theta and phi
const float theta_spacing = 0.07;
const float phi_spacing = 0.02; 

const float R1 = 1; // Inner tadius of the donut
const float R2 = 2; // Outer radius of the donut
const float K2 = 5; // Distance from the "camera" to the screen
const float K1 = screen_height * K2 * 3 / (8 * (R1+R2)); // Scaling factor for projection based on screen height 

void render_frame(float A, float B) {
    // Precompute sines and cosines of A and B
    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);

    // Create and initialize arrays to store output and z-buffer values
    char output[screen_height][screen_width];
    float zbuffer[screen_height][screen_width];

    for (int j = 0; j < screen_height; j++) {
        for (int i = 0; i < screen_width; i++) {
            output[j][i] = ' ';
            zbuffer[j][i] = 0;
        }
    }

    // theta goes around the cross-sectional circle of a torus
    for (float theta = 0; theta < 2*pi; theta += theta_spacing) {
        // precompute sines and cosines of theta
        float costheta = cos(theta), sintheta = sin(theta);

        // phi goes around the center of revolution of a torus
        for (float phi = 0; phi < 2*pi; phi += phi_spacing) {
            // precompute sines and cosines of phi
            float cosphi = cos(phi), sinphi = sin(phi);
        
            // the x,y coordinate of the circle, before revolving
            float circlex = R2 + R1*costheta;
            float circley = R1*sintheta;

            // final 3D (x,y,z) coordinate after rotations
            float x = circlex*(cosB*cosphi + sinA*sinB*sinphi) - circley*cosA*sinB;
            float y = circlex*(sinB*cosphi - sinA*cosB*sinphi) + circley*cosA*cosB;
            float z = K2 + cosA*circlex*sinphi + circley*sinA;
            
            const float aspect_ratio = 2.4; // Adjustment for non-square characters in terminal
            float ooz = 1/z; // "one over z" for perspective division
            int xp = (int) (screen_width/2 + K1*ooz*x*aspect_ratio); // x and y projection
            int yp = (int) (screen_height/2 - K1*ooz*y);
            
            // Check if the point is within screen bounds
            if (xp >= 0 && xp < screen_width && yp >= 0 && yp < screen_height) {
                // calculate luminance
                float L = cosphi*costheta*sinB - cosA*costheta*sinphi -
                    sinA*sintheta + cosB*(cosA*sintheta - costheta*sinA*sinphi);
                
                // L ranges from -sqrt(2) to +sqrt(2). If it's < 0, the surface
                // is pointing away from us, so it isn't necessary to render it.
                if (L > 0) {
                    // test against the z-buffer
                    if (ooz > zbuffer[yp][xp]) {
                        zbuffer[yp][xp] = ooz;
                        int luminance_index = (int)(L*8);
                        // luminance_index is now in the range 0..11 (8*sqrt(2) = 11.3)
                        output[yp][xp] = ".,-~:;=!*#$@"[luminance_index];
                    }
                }
            }
        }
    }

    // dump output to the screen
    printf("\x1b[H");  // move cursor to home position
    for (int j = 0; j < screen_height; j++) {
        for (int i = 0; i < screen_width; i++) {
            putchar(output[j][i]);
        }
        putchar('\n');
    }
}


// Animation loop
int main() {

    float A = 0, B = 0; // angles for rotation
    
    // Clear screen once before starting
    printf("\x1b[2J");
    
    while (1) {  // Infinite loop
        // Render frame with current angles
        render_frame(A, B);
        
        // Increment angles for rotation, control rotation speed
        A += 0.04;
        B += 0.08;

        // Control fps
        usleep(50000);
    }
    
    return 0;
}