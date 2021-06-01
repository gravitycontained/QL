#include <ql.hpp>

enum class direction {
	up,
	down,
	left,
	right,
	none
};
enum class bounce_direction {
	x,
	y,
	none
};


namespace info {
	ql::rgb entity_color = ql::rgb(220, 220, 220);
	ql::rgb background_color = ql::rgb(40, 40, 40);
	ql::rgb center_line_color = ql::rgb(50, 50, 50);
	ql::rgb menu_entity_color = ql::rgb(100, 100, 100);

	ql::u32 player_score = 0u;
	ql::u32 opponent_score = 0u;
}

struct hud {
	sf::Text player_score;
	sf::Text opponent_score;
	sf::RectangleShape center_line;

	void init(sf::Font& font, ql::u32 character_size, ql::rgb color, ql::vector2f screen_dimension) {

		this->player_score.setFont(font);
		this->player_score.setCharacterSize(character_size);
		this->player_score.setFillColor(color);
		this->player_score.setPosition(120, 30);
		this->player_score.setString("0");

		this->opponent_score.setFont(font);
		this->opponent_score.setCharacterSize(character_size);
		this->opponent_score.setFillColor(color);
		this->opponent_score.setPosition(screen_dimension.x - 100, 30);
		this->opponent_score.setString("0");

		this->update_text_allignement(screen_dimension);

		this->center_line.setSize(ql::vector2f(10, screen_dimension.y));
		this->center_line.setPosition((screen_dimension.x - this->center_line.getSize().x) / 2, 0);
		this->center_line.setFillColor(info::center_line_color);
	}

	void update_text_allignement(ql::vector2f screen_dimension) {
		auto hitbox = ql::text_hitbox(this->opponent_score);
		auto distance = screen_dimension.x - (hitbox.position.x + hitbox.dimension.x);
		auto difference = (this->player_score.getPosition().x - distance);
		this->opponent_score.move(-difference, 0);
	}
	void draw(ql::draw_object& draw_object) const {
		draw_object.draw(this->player_score);
		draw_object.draw(this->opponent_score);
		draw_object.draw(this->center_line);
	}
};

struct rectangle {
	ql::vector2f position;
	ql::vector2f dimension;
	bounce_direction last_direction;
	ql::f64 velocity = 300;
	ql::rgb color = info::entity_color;

	void make_shape() {
		this->shape.setPosition(this->position);
		this->shape.setSize(this->dimension);
		this->shape.setFillColor(this->color);
	}
	void move(ql::f64 frame_time, direction direction, ql::vector2f screen_dimension) {
		if (direction == direction::up) {
			this->position.y -= frame_time * this->velocity;

			if (this->position.y < 0) this->position.y = 0;

			this->shape.setPosition(this->position);
		}
		else if (direction == direction::down) {
			this->position.y += frame_time * this->velocity;

			if (this->position.y > screen_dimension.y - this->dimension.y) this->position.y = screen_dimension.y - this->dimension.y;

			this->shape.setPosition(this->position);
		}
	}
	void move_to_ball(ql::f64 frame_time, ql::vector2f ball_position, ql::vector2f screen_dimension) {
		auto center_y = (this->position.y + this->dimension.y / 2);

		if (center_y < ball_position.y) {
			this->move(frame_time, direction::down, screen_dimension);
		}
		else if (center_y > ball_position.y) {
			this->move(frame_time, direction::up, screen_dimension);
		}
	}

	void draw(ql::draw_object& draw_object) const {
		draw_object.draw(this->shape);
	}
	sf::RectangleShape shape;
};

struct circle {

	ql::f32 speed_over_time = 500;
	ql::vector2f position;
	ql::vector2f velocity = { speed_over_time, 250 };
	ql::rgb color = info::entity_color;
	ql::f64 cooldown = 0.05;
	ql::f32 radius;
	ql::clock respawn_cooldown;
	ql::f64 respawn_duration = 1.0;

	void make_shape() {
		this->shape.setRadius(this->radius);
		this->shape.setOrigin(this->radius, this->radius);
		this->shape.setPosition(this->position);
		this->shape.setFillColor(this->color);
	}

	void move(ql::f64 frame_time) {
		if (this->respawn_cooldown.elapsed_f() > this->respawn_duration) {
			this->position -= this->velocity * frame_time;
		}
	}
	bool is_bounce_x_allowed() const {
		return this->bounce_cooldown_x.elapsed_f() > this->cooldown;
	}
	bool is_bounce_y_allowed() const {
		return this->bounce_cooldown_y.elapsed_f() > this->cooldown;
	}
	void update_cooldown() {
		if (this->respawn_cooldown.elapsed_f() < this->respawn_duration) {
			auto progress = this->respawn_cooldown.elapsed_f() / this->respawn_duration;
			this->color.a = (255 * progress);
		}
	}
	void update_speed_over_time(ql::f64 frame_time) {
		this->speed_over_time += frame_time * 5;
		auto sign = this->velocity.x < 0;
		this->velocity.x = this->speed_over_time;
		if (sign) this->velocity.x *= -1;
	}
	void bounce_if_possible(bounce_direction direction, bool randomness) {
		if (direction == bounce_direction::x) {
			if (this->is_bounce_x_allowed()) {
				this->bounce_cooldown_x.reset();
				this->velocity.x *= -1;
				if (randomness) {
					this->velocity.y += ql::random(-100, 100);
				}
			}
		}
		else {
			if (this->is_bounce_y_allowed()) {
				this->bounce_cooldown_y.reset();
				this->velocity.y *= -1;
			}
		}
	}
	void respawn(ql::vector2f screen_dimension) {
		this->position = screen_dimension / 2;
		this->respawn_cooldown.reset();

		this->velocity = { this->speed_over_time, ql::random(-300, 300) };
		if (ql::random_event()) {
			this->velocity.x *= -1;
		}
	}

	direction check_collision_screen(ql::vector2f screen_dimension) {
		direction bounce = direction::none;
		if (this->position.x - this->radius < 0) {
			bounce = direction::left;
		}
		if (this->position.x + this->radius > screen_dimension.x) {
			bounce = direction::right;
		}
		if (this->position.y - this->radius < 0 || this->position.y + this->radius > screen_dimension.y) {
			this->bounce_if_possible(bounce_direction::y, false);
		}
		if (bounce != direction::none) {
			this->respawn(screen_dimension);
		}
		return bounce;
	}
	void check_collision_and_update_score(ql::vector2f screen_dimension, hud& hud) {
		auto bounce = this->check_collision_screen(screen_dimension);
		if (bounce == direction::left) {
			++info::opponent_score;
		}
		if (bounce == direction::right) {
			++info::player_score;
		}

		if (bounce != direction::none) {
			hud.player_score.setString(ql::to_string(info::player_score));
			hud.opponent_score.setString(ql::to_string(info::opponent_score));
			hud.update_text_allignement(screen_dimension);
		}
	}
	void check_collision_rectangle(rectangle& rectangle) {
		bool collision_y = (this->position.y + this->radius > rectangle.position.y && this->position.y - this->radius < (rectangle.position.y + rectangle.dimension.y));
		bool collision_x = (this->position.x + this->radius > rectangle.position.x && this->position.x - this->radius < (rectangle.position.x + rectangle.dimension.x));

		if (collision_y && collision_x) {
			if (rectangle.last_direction == bounce_direction::y) {
				this->bounce_if_possible(bounce_direction::x, true);
			}
			else if (rectangle.last_direction == bounce_direction::x) {
				this->bounce_if_possible(bounce_direction::y, true);
			}
			rectangle.last_direction = bounce_direction::none;
		}
		else if (collision_y) {
			rectangle.last_direction = bounce_direction::y;
		}
		else if (collision_x) {
			rectangle.last_direction = bounce_direction::x;
		}
		else {
			rectangle.last_direction = bounce_direction::none;
		}
	}

	void draw(ql::draw_object& draw_object) const {
		draw_object.draw(this->shape);
	}
	ql::clock bounce_cooldown_x;
	ql::clock bounce_cooldown_y;
	sf::CircleShape shape;
};

struct game_state : ql::base_state {

	void init() override {
		info::player_score = 0u;
		info::opponent_score = 0u;

		this->hud.init(this->get_font("arial"), 28, info::entity_color, this->dimension());

		this->clear_color = info::background_color;

		this->player.dimension = { 30, 250 };
		this->player.position = { 30, 30 };
		this->player.make_shape();

		this->opponent.dimension = this->player.dimension;
		this->opponent.position = { this->dimension().x - 30 - this->opponent.dimension.x, 30 };
		this->opponent.make_shape();

		this->ball.radius = 15;
		this->ball.position = this->dimension() / 2;
		this->ball.make_shape();
	}
	void updating() override {

		if (this->event.key_released(sf::Keyboard::Escape)) {
			this->pop_this_state();
		}
		
		auto frame_time = this->frame_time().secs_f();

		if (this->event.key_holding(sf::Keyboard::W)) {
			this->player.move(frame_time, direction::up, this->dimension());
		}
		if (this->event.key_holding(sf::Keyboard::S)) {
			this->player.move(frame_time, direction::down, this->dimension());
		}
		this->opponent.move_to_ball(frame_time, this->ball.position, this->dimension());
		this->opponent.make_shape();


		this->ball.update_cooldown();
		this->ball.update_speed_over_time(frame_time);
		this->ball.move(frame_time);
		this->ball.check_collision_and_update_score(this->dimension(), this->hud);
		this->ball.check_collision_rectangle(this->player);
		this->ball.check_collision_rectangle(this->opponent);
		this->ball.make_shape();
	}
	void drawing() override {
		this->draw(this->player);
		this->draw(this->opponent);
		this->draw(this->hud);
		this->draw(this->ball);
	}

	rectangle player;
	rectangle opponent;
	circle ball;
	hud hud;
};

struct button {
	sf::RectangleShape background;
	sf::Text text;
	bool hovering = false;
	bool clicked = false;



	void init(sf::Font& font, std::string text, ql::vector2f screen_dimension, ql::f32 y) {
		this->background.setSize(ql::vector2f(400, 100));
		this->background.setPosition((screen_dimension.x - this->background.getSize().x) / 2, y);
		this->background.setFillColor(info::background_color);
		this->background.setOutlineThickness(5);
		this->background.setOutlineColor(info::entity_color);

		auto center = ql::vector2f(this->background.getPosition()) + ql::vector2f(this->background.getSize()) / 2;

		this->text.setFont(font);
		this->text.setCharacterSize(40);
		this->text.setFillColor(info::entity_color);
		this->text.setString(text);

		auto hitbox = ql::text_hitbox(this->text);
		auto distance = center - (hitbox.position + hitbox.dimension / 2);
		this->text.move(distance);
	}
	void update(const ql::event_info& event) {
		this->hovering = false;
		this->clicked = false;

		auto mouse = event.mouse_position();
		this->hovering = (mouse.x > this->background.getPosition().x && mouse.x < this->background.getPosition().x + this->background.getSize().x &&
						  mouse.y > this->background.getPosition().y && mouse.y < this->background.getPosition().y + this->background.getSize().y);

		if (this->hovering) {
			this->text.setFillColor(info::entity_color.inverted());
			this->background.setFillColor(info::background_color.inverted());
			this->background.setOutlineColor(info::entity_color.inverted());
		}
		else {
			this->text.setFillColor(info::entity_color);
			this->background.setFillColor(info::background_color);
			this->background.setOutlineColor(info::entity_color);
		}

		if (this->hovering && event.left_mouse_clicked()) {
			this->clicked = true;
		}

	}
	void draw(ql::draw_object& draw_object) const {
		draw_object.draw(this->background);
		draw_object.draw(this->text);
	}
};

struct menu_state : ql::base_state {

	void init() override {
		ql::println(384, " bytes");

		this->play_button.init(this->get_font("arial"), "PLAY", this->dimension(), 200);
		this->exit_button.init(this->get_font("arial"), "EXIT", this->dimension(), 400);


		this->cpu1.color = info::menu_entity_color;
		this->cpu1.dimension = { 30, 250 };
		this->cpu1.position = { 30, 30 };
		this->cpu1.velocity = ql::random(100, 400);
		this->cpu1.make_shape();


		this->cpu2.color = info::menu_entity_color;
		this->cpu2.velocity = ql::random(100, 400);
		this->cpu2.dimension = this->cpu1.dimension;
		this->cpu2.position = { this->dimension().x - 30 - this->cpu2.dimension.x, 30 };
		this->cpu2.make_shape();

		this->ball.color = info::menu_entity_color;
		this->ball.radius = 15;
		this->ball.position = this->dimension() / 2;
		this->ball.make_shape();
	}
	void updating() override {
		this->play_button.update(this->event);
		this->exit_button.update(this->event);

		if (this->exit_button.clicked) {
			this->pop_this_state();
		}
		if (this->play_button.clicked) {
			this->add_state<game_state>();
		}


		auto frame_time = this->frame_time().secs_f();

		this->cpu1.move_to_ball(frame_time, this->ball.position, this->dimension());
		this->cpu1.make_shape();
		this->cpu2.move_to_ball(frame_time, this->ball.position, this->dimension());
		this->cpu2.make_shape();


		this->ball.update_cooldown();
		this->ball.update_speed_over_time(frame_time);
		this->ball.move(frame_time);
		this->ball.check_collision_screen(this->dimension());
		this->ball.check_collision_rectangle(this->cpu1);
		this->ball.check_collision_rectangle(this->cpu2);
		this->ball.make_shape();
	}
	void drawing() override {
		this->draw(this->cpu1);
		this->draw(this->cpu2);
		this->draw(this->ball);

		this->draw(this->play_button);
		this->draw(this->exit_button);
	}

	button play_button;
	button exit_button;

	rectangle cpu1;
	rectangle cpu2;
	circle ball;
};

int main() {
	ql::framework framework;
	framework.set_dimension({ 1280, 720 });
	framework.set_title("QL");
	framework.set_style(sf::Style::Close);
	framework.add_font("arial", "resources/arial.ttf");

	framework.add_state<menu_state>();
	framework.game_loop();
}