# QL

Quick Library. Written by Daniel Rabl (danielrabl1999@gmail.com). Created 2021 March, 10

A mini version of [QPL](https://github.com/DanielRabl/QPL)

Comes with standard utility and SFML utilities.
`#define QL_NO_SFML` to not use any SFML utilities.

# Setup
To use this library, download the zip and include `ql.hpp`.

Download [SFML](https://www.sfml-dev.org/). 
Example setup assuming SFML folder is named `SFML-2.5.1`:
Set include folder `SFML-2.5.1/include` and lib folder `SFML-2.5.1/lib`. 
Copy the binary files (`.dll` files) from `SFML-2.5.1/bin` into your project.

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

# Hello World

```
#include <ql.hpp>

struct circle {
	void draw(ql::draw_object& object) const {
		object.draw(circle);
	}
	sf::CircleShape circle;
};

struct game_state : ql::base_state {
	void init() override {
		this->c.circle.setRadius(100);
		this->c.circle.setPosition(ql::vector2f{ 200, 200 });
		this->c.circle.setFillColor(ql::rgb::red);
	}
	void updating() override {

	}
	void drawing() override {
		this->draw(this->c);
	}
	circle c;
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
