#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>
#include <thread>

#define PI 3.14159
#define PI2 PI * 2

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
long double zoom = 0.005;

sf::Vector2<long double> camera(0, 0);
sf::Vector2<long double> last_camera = camera;
sf::Vector2i last_mouse_position;

int screen_size[2] = { 1200, 800 };
sf::Vector2i piece_size(screen_size[0] / 3, screen_size[1] / 3);
int detail = 10;
long double mand_quallity_multpler = 1;

bool left_pressed = false, right_pressed = false;
bool optimization = true;

std::thread t1, t2, t3, t4, t5, t6, t7, t8, t9;

sf::Image screen_image;
sf::Texture screen_texture;
sf::Sprite screen_sprite;

sf::RectangleShape croshair(sf::Vector2f(4, 4));


long double increase_rate(long double x, long double y, int quallity) {
    int iter;
    std::complex<long double> N = 0, last_N, last_abs = 0;
    for (iter = 0; iter < quallity && abs(N) < 2; iter++) {
        last_N = N;
        N = N * N + std::complex<long double>(x, y);
    }
    if (abs(N) < 2) return 0;
    else {
        return (last_N / N).real();
        //return abs(N);
        //return (last_abs / abs(N)).real();
        //return iter / quallity;
    }
}


void piece_drawing(int x_offset, int y_offset) {
    sf::Color pixelColor;
    long double x_v, y_v;
    for (long double y = piece_size.y * y_offset; y < piece_size.y + piece_size.y * y_offset && y < screen_size[1] - detail; y += detail) {
        for (long double x = piece_size.x * x_offset; x < piece_size.x + piece_size.x * x_offset && x < screen_size[0] - detail; x += detail) {
            x_v = x - long double(screen_size[0]) / 2;
            y_v = y - long double(screen_size[1]) / 2;
            long double rate = increase_rate(zoom * long double(x_v - camera.x), zoom * long double(y_v - camera.y), int(mand_quallity_multpler * 10) + 5) * 0.1;
            pixelColor.r = 255.0 * sin(PI2 * rate + (PI2 * 0.3) + (mand_quallity_multpler * 0.1));
            pixelColor.g = 255.0 * sin(PI2 * rate + (PI2 * 0.6) + (mand_quallity_multpler * 0.1));
            pixelColor.b = 255.0 * sin(PI2 * rate + (PI2 * 0.9) + (mand_quallity_multpler * 0.1));

            for (long double i = y; i < (y + detail); i++) {
                for (long double j = x; j < (x + detail); j++) {
                    screen_image.setPixel(j, i, pixelColor);

                }
            }
        }
    }
}

int main() {

    sf::RenderWindow window(sf::VideoMode(screen_size[0], screen_size[1]), "graph");

    screen_image.create(screen_size[0] + detail, screen_size[1] + detail, sf::Color(0, 0, 0));
    croshair.setPosition(sf::Vector2f(screen_size[0] / 2 - 2, screen_size[1] / 2 - 2));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    left_pressed = false;
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    right_pressed = false;
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    left_pressed = true;
                    last_mouse_position = sf::Mouse::getPosition();
                    last_camera = camera;
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    right_pressed = true;
                }
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                detail = 10;
                optimization = true;
                if (event.mouseWheel.delta < 0) {
                    mand_quallity_multpler -= 0.033;
                    zoom *= 1.1;               
                    camera.x /= 1.1;
                    camera.y /= 1.1;
                }                              
                else {
                    mand_quallity_multpler += 0.033;
                    zoom /= 1.1;
                    camera.x *= 1.1;
                    camera.y *= 1.1;
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    detail += 1;
                }
                if (event.key.code == sf::Keyboard::Right) {
                    if (detail > 1) detail -= 1;
                }
            }
        }


        ////// Controls
        if (left_pressed) {
            camera.x = last_camera.x - last_mouse_position.x + sf::Mouse::getPosition().x;
            camera.y = last_camera.y - last_mouse_position.y + sf::Mouse::getPosition().y;
        }


        ////// Logic
        long double camera_x = long double(camera.x);
        long double camera_y = long double(camera.y);
        if (left_pressed) {
            detail = 10;
            optimization = true;
        }
        else {
            if (detail > 1) detail -= 1;
        }
        

        ////// Drawing
        //sf::RectangleShape rect;
        //rect.setSize(sf::Vector2f(detail, detail));
        //sf::Color pixelColor;

        if (optimization) {
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

            
            screen_texture.loadFromImage(screen_image);
            screen_sprite.setTexture(screen_texture);


            if (detail <= 1) optimization = false;
        }
        window.draw(screen_sprite);
        window.draw(croshair);
        window.display();
    }

    return 0;
}