#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>

template <class T>
class Vector4 {
public:
    T x, y, z, i;
    
    Vector4 () {
        x = 0;
        y = 0;
        z = 0;
        i = 0;
    }
    Vector4 (T x_v, T y_v, T z_v, T i_v) {
        x = x_v;
        y = y_v;
        z = z_v;
        i = i_v;
    }
};


// Some necessary global variables
double zoom = 0.01;

sf::Vector2<double> camera(0, 0);
sf::Vector2<double> last_camera = camera;
sf::Vector2i last_mouse_position;

int screen_size[2] = { 800, 600 };
sf::Vector2i piece_size(screen_size[0] / 3, screen_size[1] / 3);
int detail = 1;

bool left_pressed = false, right_pressed = false;

std::thread t1, t2, t3, t4, t5, t6, t7, t8, t9;

sf::Image screen_image;


double complex_abs(double d, double m) {
    return sqrt(d * d + m * m);
}


int increase_rate(double x, double y) {
    double d = 0, m = 0, z = 0;
    for (int iter = 0; iter < 30 && complex_abs(d, m) < 2; iter++) {
        d = d * d + x;
        m += y;
    }
    if (complex_abs(d, m) < 2) return 255;
    else return 0;
}


void piece_drawing(int x_offset, int y_offset) {
    sf::Color pixelColor;

    for (int y = 0; y < piece_size.y; y += detail) {
        for (int x = 0; x < piece_size.x; x += detail) {
            pixelColor.r = 0;
            pixelColor.g = 0;
            pixelColor.b = increase_rate(zoom * double(x - camera.x + piece_size.x * x_offset), zoom * double(y - camera.y + piece_size.y * y_offset)) ;

            for (int i = y; i < (y + detail); i++) {
                for (int j = x; j < (x + detail); j++) {
                    screen_image.setPixel(j + (x_offset * piece_size.x), i + (y_offset * piece_size.y), pixelColor);

                }
            }
        }
    }
}

int main() {

    sf::RenderWindow window(sf::VideoMode(screen_size[0], screen_size[1]), "graph");

    screen_image.create(screen_size[0] + detail, screen_size[1] + detail, sf::Color(0, 0, 0));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    left_pressed = false;
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    right_pressed = false;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    left_pressed = true;
                    last_mouse_position = sf::Mouse::getPosition();
                    last_camera = camera;
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    right_pressed = true;
                }
            }
            if (event.type == sf::Event::MouseWheelMoved)
            {
                if (event.mouseWheel.delta < 0) {
                    zoom *= 1.1;               
                    camera.x /= 1.1;           
                    camera.y /= 1.1;           
                }                              
                else {                         
                    zoom /= 1.1;
                    camera.x *= 1.1;
                    camera.y *= 1.1;
                }
            }
        }


        ////// Controls
        if (left_pressed) {
            camera.x = last_camera.x - last_mouse_position.x + sf::Mouse::getPosition().x;
            camera.y = last_camera.y - last_mouse_position.y + sf::Mouse::getPosition().y;
        }


        ////// Logic
        int camera_x = int(camera.x);
        int camera_y = int(camera.y);
        

        ////// Drawing
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(detail, detail));
        sf::Color pixelColor;

        t1 = std::thread(piece_drawing, 0, 0);
        t1.join();
        t2 = std::thread(piece_drawing, 0, 1);
        t2.join();
        t3 = std::thread(piece_drawing, 0, 2);
        t3.join();
        t4 = std::thread(piece_drawing, 1, 0);
        t4.join();
        t5 = std::thread(piece_drawing, 1, 1);
        t5.join();
        t6 = std::thread(piece_drawing, 1, 2);
        t6.join();
        t7 = std::thread(piece_drawing, 2, 0);
        t7.join();
        t8 = std::thread(piece_drawing, 2, 1);
        t8.join();
        t9 = std::thread(piece_drawing, 2, 2);
        t9.join();

        sf::Texture screen_texture;
        screen_texture.loadFromImage(screen_image);
        sf::Sprite screen_sprite;

        screen_sprite.setTexture(screen_texture);
        window.draw(screen_sprite);

        window.display();
    }

    return 0;
}