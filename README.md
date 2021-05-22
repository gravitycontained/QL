# QL

Written by Daniel Rabl (danielrabl1999@gmail.com). Created 2021 March, 10

A mini version of [QPL](https://github.com/DanielRabl/QPL)

Comes with standard utility and SFML utilities.
`#define QL_NO_SFML` to not use any SFML utilities.

# Setup
To use this library, download the zip and include `ql.hpp`. Set the C++ Standard to latest version to `/std:c++20` or `/std:c++latest`.

Download latest [SFML](https://www.sfml-dev.org/) version. Look for 64 bit version if possible. 

Example setup assuming SFML folder is named `SFML-2.5.1`:
Set include folder `SFML-2.5.1/include` and library folder `SFML-2.5.1/lib`. 
Copy the binary files (`.dll` files) from `SFML-2.5.1/bin` into your project folder.

Add linker dependencies:

release x86/x64:
```
sfml-graphics.lib
sfml-window.lib
sfml-main.lib
sfml-system.lib
sfml-audio.lib
sfml-network.lib
```

debug x86/x64:
```
sfml-graphics-d.lib
sfml-window-d.lib
sfml-main-d.lib
sfml-system-d.lib
sfml-audio-d.lib
sfml-network-d.lib
```

# SFML Hello World

```
#include <SFML/Graphics.hpp>

int main() {
	auto dimension = sf::VideoMode(1280u, 720u);
	auto title = "hello world";
	sf::RenderWindow window;
	window.create(dimension, title);

	sf::CircleShape circle;
	circle.setRadius(100);
	circle.setPosition(200, 200);
	circle.setFillColor(sf::Color::Red);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear();
		window.draw(circle);
		window.display();
	}
}
```

# QL Hello World

```
#include <ql.hpp>

struct game_state : ql::base_state {
	void init() override {
		this->circle.setRadius(100);
		this->circle.setPosition(ql::vector2f{ 200, 200 });
		this->circle.setFillColor(ql::rgb::red);
	}
	void updating() override {

	}
	void drawing() override {
		this->draw(this->circle);
	}
	sf::CircleShape circle;
};

int main() {

	ql::framework framework;
	framework.set_title("hello world");
	framework.set_dimension({ 1280u, 720u });
	
	framework.add_state<game_state>();
	framework.game_loop();
}
```

compile and execute and you should see this:

![Red Circle on Black Background](https://i.imgur.com/jahBJvQ.png)
