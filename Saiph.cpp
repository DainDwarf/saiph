#include "Saiph.h"

/* constructors */
Saiph::Saiph(int interface) {
	debugfile.open("debug.log", ios::trunc);
	connection = Connection::create(interface, &debugfile);
	if (connection == NULL) {
		cout << "ERROR: Don't know what interface this is: " << interface << endl;
		exit(1);
	}
	world = new World(connection, &debugfile);

	/* engulfed */
	engulfed = false;

	/* set best_priority to ILLEGAL_PRIORITY */
	best_priority = ILLEGAL_PRIORITY;

	/* set on_ground to NULL */
	on_ground = NULL;

	/* tracking, pathing & maps */
	for (int a = 0; a <= UCHAR_MAX; ++a) {
		/* monstertracking */
		if ((a >= '@' && a <= 'Z') || (a >= 'a' && a <= 'z') || (a >= '1' && a <= '5')  || a == '&' || a == '\'' || a == ':' || a == ';' || a == '~' || a == PET)
			monster[a] = true;
		else
			monster[a] = false;
		/* itemtracking */
		item[a] = false;
		/* dungeon symbol tracking */
		track_symbol[a] = false;
		/* pathing & maps */
		passable[a] = false;
		dungeon[a] = false;
		pathcost[a] = 0;
	}
	/* items */
	item[(unsigned char) WEAPON] = true;
	item[(unsigned char) ARMOR] = true;
	item[(unsigned char) RING] = true;
	item[(unsigned char) AMULET] = true;
	item[(unsigned char) TOOL] = true;
	item[(unsigned char) FOOD] = true;
	item[(unsigned char) POTION] = true;
	item[(unsigned char) SCROLL] = true;
	item[(unsigned char) SPELLBOOK] = true;
	item[(unsigned char) WAND] = true;
	item[(unsigned char) GOLD] = true;
	item[(unsigned char) GEM] = true;
	item[(unsigned char) STATUE] = true;
	// skipping boulder as that's a special item
	item[(unsigned char) IRON_BALL] = true;
	item[(unsigned char) CHAINS] = true;
	item[(unsigned char) VENOM] = true;
	/* dungeon symbol tracking */
	track_symbol[(unsigned char) OPEN_DOOR] = true;
	track_symbol[(unsigned char) CLOSED_DOOR] = true;
	track_symbol[(unsigned char) STAIRS_UP] = true;
	track_symbol[(unsigned char) STAIRS_DOWN] = true;
	track_symbol[(unsigned char) ALTAR] = true;
	track_symbol[(unsigned char) GRAVE] = true;
	track_symbol[(unsigned char) THRONE] = true;
	track_symbol[(unsigned char) SINK] = true;
	track_symbol[(unsigned char) FOUNTAIN] = true;
	track_symbol[(unsigned char) VERTICAL_WALL] = true;
	/* pathing & maps */
	passable[(unsigned char) FLOOR] = true;
	passable[(unsigned char) OPEN_DOOR] = true;
	passable[(unsigned char) CORRIDOR] = true;
	passable[(unsigned char) STAIRS_UP] = true;
	passable[(unsigned char) STAIRS_DOWN] = true;
	passable[(unsigned char) ALTAR] = true;
	passable[(unsigned char) GRAVE] = true;
	passable[(unsigned char) THRONE] = true;
	passable[(unsigned char) SINK] = true;
	passable[(unsigned char) FOUNTAIN] = true;
	passable[(unsigned char) WATER] = true;
	passable[(unsigned char) ICE] = true;
	passable[(unsigned char) LAVA] = true;
	passable[(unsigned char) LOWERED_DRAWBRIDGE] = true;
	passable[(unsigned char) TRAP] = true;
	passable[(unsigned char) UNKNOWN_TILE] = true;
	passable[(unsigned char) UNKNOWN_TILE_DIAGONALLY_PASSABLE] = true;
	passable[(unsigned char) WEAPON] = true;
	passable[(unsigned char) ARMOR] = true;
	passable[(unsigned char) RING] = true;
	passable[(unsigned char) AMULET] = true;
	passable[(unsigned char) TOOL] = true;
	passable[(unsigned char) FOOD] = true;
	passable[(unsigned char) POTION] = true;
	passable[(unsigned char) SCROLL] = true;
	passable[(unsigned char) SPELLBOOK] = true;
	passable[(unsigned char) WAND] = true;
	passable[(unsigned char) GOLD] = true;
	passable[(unsigned char) GEM] = true;
	passable[(unsigned char) STATUE] = true;
	passable[(unsigned char) IRON_BALL] = true;
	passable[(unsigned char) CHAINS] = true;
	passable[(unsigned char) VENOM] = true;
	passable[(unsigned char) PET] = true;
	dungeon[(unsigned char) VERTICAL_WALL] = true;
	dungeon[(unsigned char) HORIZONTAL_WALL] = true;
	dungeon[(unsigned char) FLOOR] = true;
	dungeon[(unsigned char) OPEN_DOOR] = true;
	dungeon[(unsigned char) CLOSED_DOOR] = true;
	dungeon[(unsigned char) IRON_BARS] = true;
	dungeon[(unsigned char) TREE] = true;
	dungeon[(unsigned char) CORRIDOR] = true;
	dungeon[(unsigned char) STAIRS_UP] = true;
	dungeon[(unsigned char) STAIRS_DOWN] = true;
	dungeon[(unsigned char) ALTAR] = true;
	dungeon[(unsigned char) GRAVE] = true;
	dungeon[(unsigned char) THRONE] = true;
	dungeon[(unsigned char) SINK] = true;
	dungeon[(unsigned char) FOUNTAIN] = true;
	dungeon[(unsigned char) WATER] = true;
	dungeon[(unsigned char) ICE] = true;
	dungeon[(unsigned char) LAVA] = true;
	dungeon[(unsigned char) LOWERED_DRAWBRIDGE] = true;
	dungeon[(unsigned char) RAISED_DRAWBRIDGE] = true;
	dungeon[(unsigned char) TRAP] = true;
	dungeon[(unsigned char) BOULDER] = true; // hardly static, but we won't allow moving on to one
	pathcost[(unsigned char) ICE] = COST_ICE;
	pathcost[(unsigned char) LAVA] = COST_LAVA;
	pathcost[(unsigned char) PET] = COST_PET;
	pathcost[(unsigned char) TRAP] = COST_TRAP;
	pathcost[(unsigned char) WATER] = COST_WATER;

	/* Analyzers */
	analyzers.push_back(new Beatitude(this));
	analyzers.push_back(new Door(this));
	analyzers.push_back(new Elbereth(this));
	analyzers.push_back(new Enhance(this));
	analyzers.push_back(new Excalibur(this));
	analyzers.push_back(new Explore(this));
	analyzers.push_back(new Fight(this));
	analyzers.push_back(new Food(this));
	analyzers.push_back(new Health(this));
	//analyzers.push_back(new Level(this));
	analyzers.push_back(new Loot(this));
	analyzers.push_back(new Pray(this));
	analyzers.push_back(new Wand(this));
	analyzers.push_back(new Wish(this));
	analyzers.push_back(new Zorkmid(this));
}

/* destructor */
Saiph::~Saiph() {
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		delete *a;
	delete world;
	delete connection;
	debugfile.close();
}

/* methods */
unsigned char Saiph::directLine(Point point, bool ignore_sinks) {
	/* is the target in a direct line from the player? */
	if (point.row < MAP_ROW_BEGIN || point.row > MAP_ROW_END || point.col < MAP_COL_BEGIN || point.col > MAP_COL_END) {
		/* outside map */
		return ILLEGAL_MOVE;
	} else if (point == position) {
		/* eh? this doesn't happen */
		return REST;
	} else if (point.row == position.row) {
		/* aligned horizontally */
		if (point.col > position.col) {
			while (--point.col > position.col) {
				if (!directLineHelper(point, ignore_sinks))
					return ILLEGAL_MOVE;
			}
			return MOVE_E;
		} else {
			while (++point.col < position.col) {
				if (!directLineHelper(point, ignore_sinks))
					return ILLEGAL_MOVE;
			}
			return MOVE_W;
		}
	} else if (point.col == position.col) {
		/* aligned vertically */
		if (point.row > position.row) {
			while (--point.row > position.row) {
				if (!directLineHelper(point, ignore_sinks))
					return ILLEGAL_MOVE;
			}
			return MOVE_S;
		} else {
			while (++point.row < position.row) {
				if (!directLineHelper(point, ignore_sinks))
					return ILLEGAL_MOVE;
			}
			return MOVE_N;
		}
	} else if (abs(point.row - position.row) == abs(point.col - position.col)) {
		/* aligned diagonally */
		if (point.row > position.row) {
			if (point.col > position.col) {
				while (--point.row > position.row) {
					--point.col;
					if (!directLineHelper(point, ignore_sinks))
						return ILLEGAL_MOVE;
				}
				return MOVE_SE;
			} else {
				while (--point.row > position.row) {
					++point.col;
					if (!directLineHelper(point, ignore_sinks))
						return ILLEGAL_MOVE;
				}
				return MOVE_SW;
			}
		} else {
			if (point.col > position.col) {
				while (++point.row < position.row) {
					--point.col;
					if (!directLineHelper(point, ignore_sinks))
						return ILLEGAL_MOVE;
				}
				return MOVE_NE;
			} else {
				while (++point.row < position.row) {
					++point.col;
					if (!directLineHelper(point, ignore_sinks))
						return ILLEGAL_MOVE;
				}
				return MOVE_NW;
			}
		}
	}
	return ILLEGAL_MOVE;
}

void Saiph::farlook(const Point &target) {
	/* look at something, eg. monster */
	/* TODO: make this an analyzer */
	string command;
	command.push_back(';');
	Point cursor(world->player.row, world->player.col);
	while (cursor.row != target.row && cursor.col != target.col) {
		unsigned char move;
		if (cursor.row < target.row && cursor.col < target.col) {
			move = MOVE_SE;
			++cursor.row;
			++cursor.col;
		} else if (cursor.row < target.row && cursor.col > target.col) {
			move = MOVE_SW;
			++cursor.row;
			--cursor.col;
		} else if (cursor.row > target.row && cursor.col < target.col) {
			move = MOVE_NE;
			--cursor.row;
			++cursor.col;
		} else if (cursor.row > target.row && cursor.col > target.col) {
			move = MOVE_NW;
			--cursor.row;
			--cursor.col;
		} else if (cursor.row < target.row) {
			move = MOVE_S;
			++cursor.row;
		} else if (cursor.row > target.row) {
			move = MOVE_N;
			--cursor.row;
		} else if (cursor.col < target.col) {
			move = MOVE_E;
			++cursor.col;
		} else {
			move = MOVE_W;
			--cursor.col;
		}
		command.push_back(move);
	}
	command.push_back(',');
	world->executeCommand(command);
}

void Saiph::removeItemFromInventory(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	map<unsigned char, Item>::iterator i = inventory.find(key);
	if (i == inventory.end())
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from inventory slot " << key << endl;
	if (i->second.count > item.count)
		i->second.count -= item.count; // we got more than we remove
	else
		inventory.erase(i); // removing all we got
}

void Saiph::removeItemFromStash(const Point &point, const Item &item) {
	if (item.count <= 0)
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from stash at " << position.level << ", " << point.row << ", " << point.col << endl;
	map<Point, Stash>::iterator s = levels[position.level].stashes.find(point);
	if (s != levels[position.level].stashes.end()) {
		s->second.removeItem(item);
		if (s->second.items.size() <= 0)
			levels[position.level].stashes.erase(point);
	}
}

bool Saiph::request(const Request &request) {
	/* request an action from any analyzer */
	debugfile << REQUEST_DEBUG_NAME << request.request << ", " << request.priority << ", " << request.value << ", " << request.data << ", (" << request.coordinate.level << ", " << request.coordinate.row << ", " << request.coordinate.col << ")" << endl;
	bool status = false;
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
		if ((*a)->request(request) && !status)
			status = true;
	}
	return status;
}

bool Saiph::run() {
	/* clear pickup list */
	pickup.clear();
	/* and on_ground */
	on_ground = NULL;

	/* check if we're engulfed */
	if (position.row > MAP_ROW_BEGIN && position.row < MAP_ROW_END && position.col > MAP_COL_BEGIN && position.col < MAP_COL_END && world->view[position.row - 1][position.col - 1] == '/' && world->view[position.row - 1][position.col + 1] == '\\' && world->view[position.row + 1][position.col - 1] == '\\' && world->view[position.row + 1][position.col + 1] == '/')
		engulfed = true;
	else
		engulfed = false;

	/* detect player position */
	detectPosition();

	/* clear priority from analyzers */
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		(*a)->priority = ILLEGAL_PRIORITY;

	/* deal with messages */
	debugfile << MESSAGES_DEBUG_NAME << "'" << world->messages << "'" << endl;
	/* global parsing */
	parseMessages();

	/* update maps */
	if (!world->question && !world->menu && !engulfed)
		updateMaps();
	/* print maps so we see what we're doing */
	dumpMaps();

	Analyzer *best_analyzer = NULL;
	best_priority = ILLEGAL_PRIORITY;

	/* then analyzer message parsing */
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
		(*a)->parseMessages(world->messages);
		if ((*a)->priority > best_priority) {
			best_priority = (*a)->priority;
			best_analyzer = *a;
		}
	}

	/* call begin() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
			(*a)->begin();
			if ((*a)->priority > best_priority) {
				best_priority = (*a)->priority;
				best_analyzer = *a;
			}
		}
	}

	/* inspect the dungeon */
	if (!world->question && !world->menu) {
		for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
			for (vector<Point>::iterator c = world->changes.begin(); c != world->changes.end(); ++c) {
				(*a)->inspect(*c);
			if ((*a)->priority > best_priority) {
				best_priority = (*a)->priority;
				best_analyzer = *a;
			}
		}
	}

	/* call finish() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
			(*a)->finish();
			if ((*a)->priority > best_priority) {
				best_priority = (*a)->priority;
				best_analyzer = *a;
			}
		}
	}

	/* check if we got a command */
	if (world->question && best_analyzer == NULL) {
		debugfile << SAIPH_DEBUG_NAME << "Unhandled question: " << world->messages << endl;
		return false;
	} else if (world->menu && best_analyzer == NULL) {
		debugfile << SAIPH_DEBUG_NAME << "Unhandled menu: " << world->messages << endl;
		return false;
	} else if (best_analyzer == NULL) {
		debugfile << SAIPH_DEBUG_NAME << "I have no idea what to do... Searching" << endl;
		world->executeCommand("s");
		return true;
	}

	/* let an analyzer do its command */
	debugfile << COMMAND_DEBUG_NAME << "'" << best_analyzer->command << "' from analyzer " << best_analyzer->name << " with priority " << best_priority << endl;
	world->executeCommand(best_analyzer->command);
	best_analyzer->complete();
	return true;
}

void Saiph::setDungeonSymbolValue(const Point &point, unsigned char symbol, int value) {
	/* set the value of a certain dungeon symbol at a certain point.
	 * we can use this to set [un]locked doors, alignment of altars, etc */
	if (!track_symbol[symbol])
		return;
	map<Point, int>::iterator d = levels[position.level].symbols[symbol].find(point);
	if (d == levels[position.level].symbols[symbol].end())
		return;
	d->second = value;
}

unsigned char Saiph::shortestPath(unsigned char symbol, bool allow_illegal_last_move, int *moves) {
	/* returns next move in shortest path from player to nearest symbol.
	 * also sets amount of moves to the target */
	if (!track_symbol[symbol])
		return ILLEGAL_MOVE;
	int least_moves = INT_MAX;
	unsigned char best_move = ILLEGAL_MOVE;
	int level = 0;
	for (vector<Level>::iterator l = levels.begin(); l != levels.end(); ++l) {
		++level;
		for (map<Point, int>::iterator p = l->symbols[symbol].begin(); p != l->symbols[symbol].end(); ++p) {
			unsigned char move = shortestPath(Coordinate(level, p->first), allow_illegal_last_move, moves);
			if (move == ILLEGAL_MOVE)
				continue;
			if (level != position.level)
				*moves += abs(level - position.level) * 100; // on another level, add 100 moves per level
			if (*moves >= least_moves)
				continue;
			least_moves = *moves;
			best_move = move;
		}
	}
	return best_move;
}

unsigned char Saiph::shortestPath(const Coordinate &target, bool allow_illegal_last_move, int *moves) {
	/* returns next move in shortest path from player to target.
	 * also sets amount of moves to the target */
	if (target.level < 0 || target.level >= (int) levels.size())
		return ILLEGAL_MOVE; // outside the map
	if (target.level < position.level) {
		/* path to upstairs */
		for (map<Point, int>::iterator s = levels[position.level].symbols[STAIRS_UP].begin(); s != levels[position.level].symbols[STAIRS_UP].end(); ++s) {
			/* need to check that these are the stairs we want */
			unsigned char move = doPath(s->first, allow_illegal_last_move, moves);
			if (move == REST)
				return MOVE_UP;
			return move;
		}
	} else if (target.level > position.level) {
		/* path to downstairs */
		for (map<Point, int>::iterator s = levels[position.level].symbols[STAIRS_DOWN].begin(); s != levels[position.level].symbols[STAIRS_DOWN].end(); ++s) {
			/* need to check that these are the stairs we want */
			unsigned char move = doPath(s->first, allow_illegal_last_move, moves);
			if (move == REST)
				return MOVE_DOWN;
			return move;
		}
	} else {
		/* path to target */
		return doPath(target, allow_illegal_last_move, moves);
	}
	return ILLEGAL_MOVE;
}

unsigned char Saiph::shortestPath(const Point &target, bool allow_illegal_last_move, int *moves) {
	/* pathing on same level */
	return doPath(target, allow_illegal_last_move, moves);
}

/* private methods */
void Saiph::addItemToInventory(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to inventory slot " << key << endl;
	if (inventory.find(key) != inventory.end()) {
		/* existing item, add amount */
		inventory[key].count += item.count;
	} else {
		/* new item */
		inventory[key] = item;
	}
}

void Saiph::addItemToStash(const Point &point, const Item &item) {
	if (item.count <= 0)
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to stash at " << position.level << ", " << point.row << ", " << point.col << endl;
	map<Point, Stash>::iterator s = levels[position.level].stashes.find(point);
	if (s != levels[position.level].stashes.end()) {
		s->second.addItem(item);
		return;
	}
	/* new stash */
	Stash stash(world->player.turn);
	stash.items.push_back(item);
	levels[position.level].stashes[point] = stash;
}

void Saiph::clearStash(const Point &point) {
	/* clear the contents of a stash */
	debugfile << ITEMTRACKER_DEBUG_NAME << "Clearing stash at " << position.level << ", " << point.row << ", " << point.col << endl;
	map<Point, Stash>::iterator s = levels[position.level].stashes.find(point);
	if (s != levels[position.level].stashes.end())
		s->second.items.clear();
}

void Saiph::detectPosition() {
	if (position.level == -1) {
		/* this happens when we start */
		position.row = world->player.row;
		position.col = world->player.col;
		position.level = levels.size();
		levels.push_back(Level(world->player.level));
		levelmap[world->player.level].push_back(position.level);
		return;
	}
	if ((int) levels.size() >= position.level - 1 && strcmp(world->player.level, levels[position.level].name.c_str()) == 0) {
		/* same level as last frame, just update row & col */
		position.row = world->player.row;
		position.col = world->player.col;
		return;
	}
	/* level has changed.
	 * we need to figure out if it's a new level or one we already know of */
	int found = -1;
	/* maybe we already know where these stairs lead? */
	if (levels[position.level].dungeonmap[position.row][position.col] == STAIRS_DOWN) {
		/* we did stand on stairs down, and if we don't know where they lead then
		 * the next line will still just set found to -1 */
		found = levels[position.level].symbols[STAIRS_DOWN][position];
	} else if (levels[position.level].dungeonmap[position.row][position.col] == STAIRS_UP) {
		/* we did stand on stairs up, and if we don't know where they lead then
		 * the next line will still just set found to -1 */
		found = levels[position.level].symbols[STAIRS_UP][position];
	}
	string level = world->player.level;
	if (found == -1) {
		/* we didn't know where the stairs would take us */
		for (vector<int>::iterator lm = levelmap[level].begin(); lm != levelmap[level].end(); ++lm) {
			/* check if level got vertical walls on same locations.
			 * since walls can disappear, we'll allow a 70% match */
			int total = 0;
			int matched = 0;
			for (map<Point, int>::iterator s = levels[*lm].symbols[VERTICAL_WALL].begin(); s != levels[*lm].symbols[VERTICAL_WALL].end(); ++s) {
				if (world->view[s->first.row][s->first.col] == VERTICAL_WALL)
					++matched;
				++total;
			}
			if (matched * 10 >= total * 7) {
				found = *lm;
				break;
			}
		}
	}
	if (found == -1) {
		/* new level */
		found = levels.size();
		levels.push_back(Level(level));
	}
	/* were we on stairs on last position? */
	if (levels[position.level].dungeonmap[position.row][position.col] == STAIRS_DOWN) {
		/* yes, we were on stairs down */
		levels[position.level].symbols[STAIRS_DOWN][position] = found;
	} else if (levels[position.level].dungeonmap[position.row][position.col] == STAIRS_UP) {
		/* yes, we were on stairs up */
		levels[position.level].symbols[STAIRS_UP][position] = found;
	}
	position.row = world->player.row;
	position.col = world->player.col;
	position.level = found;
	levelmap[level].push_back(found);
}

bool Saiph::directLineHelper(const Point &point, bool ignore_sinks) {
	if (!passable[levels[position.level].dungeonmap[point.row][point.col]])
		return false;
	else if (!ignore_sinks && levels[position.level].dungeonmap[point.row][point.col] == SINK)
		return false;
	else if (levels[position.level].monstermap[point.row][point.col] != ILLEGAL_MONSTER && levels[position.level].monsters[point].visible)
		return false;
	return true;
}

unsigned char Saiph::doPath(const Point &target, bool allow_illegal_last_move, int *moves) {
	/* returns next move in shortest path from player to target.
	 * also sets "moves" to target */
	if (target.row < MAP_ROW_BEGIN || target.row > MAP_ROW_END || target.col < MAP_COL_BEGIN || target.col > MAP_COL_END)
		return ILLEGAL_MOVE; // outside the map
	PathNode *node = &pathmap[target.row][target.col];
	*moves = 0;
	if (node->cost == 0)
		return REST; // pathing to player?
	++*moves;
	unsigned char move = ILLEGAL_MOVE;
	if (allow_illegal_last_move && node->nextnode == NULL) {
		/* sometimes we wish to move somewhere we really can't move to.
		 * for example: fighting a monster in a wall or through "corner".
		 * solution: find adjacent squares with lowest cost and backtrack from there */
		/* note:
		 * since we're moving from target to player,
		 * we're moving the opposite direction of the node we're checking */
		/* northwest node */
		int row = target.row - 1;
		int col = target.col - 1;
		move = MOVE_SE;
		node = &pathmap[row][col];
		unsigned int lowest_cost = node->cost;
		/* north node */
		++col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_S;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* northeast node */
		++col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_SW;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* east node */
		++row;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_W;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* southeast node */
		++row;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_NW;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* south node */
		--col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_N;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* southwest node */
		--col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_NE;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* west node */
		--row;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_E;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		if (lowest_cost == 0)
			return move; // found the player
		++*moves;
	}
	if (node->nextnode == NULL)
		return ILLEGAL_MOVE; // couldn't find path

	while (node->nextnode != NULL) {
		move = node->move;
		++*moves;
		node = node->nextnode;
	}
	return move;
}

void Saiph::dumpMaps() {
	/* monsters */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";2H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (levels[position.level].monstermap[r][c] != ILLEGAL_MONSTER)
				cout << (unsigned char) (levels[position.level].monstermap[r][c]);
			else
				cout << (unsigned char) (levels[position.level].dungeonmap[r][c]);
		}
	}
	/* world map as the bot sees it */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 1 << ";82H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m";
			cout << (unsigned char) (levels[position.level].dungeonmap[r][c]);
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[m";
		}
	}
	/* path map */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";82H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (pathmap[r][c].move >= 'a' && pathmap[r][c].move <= 'z')
				cout << (unsigned char) pathmap[r][c].move;
			else
				cout << (unsigned char) (levels[position.level].dungeonmap[r][c]);
		}
	}
	/* return cursor back to where it was */
	cout << (unsigned char) 27 << "[" << world->cursor.row + 1 << ";" << world->cursor.col + 1 << "H";
	/* and flush cout. if we don't do this our output looks like garbage */
	cout.flush();
}

void Saiph::parseMessages() {
	/* parse messages that can help us find doors/staircases/etc. */
	if (world->messages.find(MESSAGE_STAIRCASE_UP_HERE, 0) != string::npos)
		setDungeonSymbol(position, STAIRS_UP);
	else if (world->messages.find(MESSAGE_STAIRCASE_DOWN_HERE, 0) != string::npos)
		setDungeonSymbol(position, STAIRS_DOWN);
	else if (world->messages.find(MESSAGE_OPEN_DOOR_HERE, 0) != string::npos)
		setDungeonSymbol(position, OPEN_DOOR);
	else if (world->messages.find(MESSAGE_FOUNTAIN_HERE, 0) != string::npos)
		setDungeonSymbol(position, FOUNTAIN);
	else if (world->messages.find(MESSAGE_FOUNTAIN_DRIES_UP, 0) != string::npos || world->messages.find(MESSAGE_FOUNTAIN_DRIES_UP2, 0) != string::npos)
		setDungeonSymbol(position, FLOOR);
	/* when we've checked messages for static dungeon features and not found anything,
	 * then we can set the tile to UNKNOWN_TILE_DIAGONALLY_PASSABLE if the tile is UNKNOWN_TILE */
	else if (levels[position.level].dungeonmap[world->player.row][world->player.col] == UNKNOWN_TILE)
		setDungeonSymbol(position, UNKNOWN_TILE_DIAGONALLY_PASSABLE);

	/* item parsing */
	/* figure out if there's something on the ground or if we're picking up something */
	string::size_type pos;
	if ((pos = world->messages.find(MESSAGE_YOU_SEE_HERE, 0)) != string::npos || (pos = world->messages.find(MESSAGE_YOU_FEEL_HERE, 0)) != string::npos) {
		/* single item on ground */
		clearStash(position);
		pos += sizeof (MESSAGE_YOU_SEE_HERE) - 1;
		string::size_type length = world->messages.find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			Item item(world->messages.substr(pos, length));
			addItemToStash(position, item);
		}
		/* if there are no items in this stash, erase it */
		if (levels[position.level].stashes[position].items.size() <= 0)
			levels[position.level].stashes.erase(position);
	} else if ((pos = world->messages.find(MESSAGE_THINGS_THAT_ARE_HERE, 0)) != string::npos || (pos = world->messages.find(MESSAGE_THINGS_THAT_ARE_HERE, 0)) != string::npos) {
		/* multiple items on ground */
		clearStash(position);
		pos = world->messages.find("  ", pos + 1);
		while (pos != string::npos && world->messages.size() > pos + 2) {
			pos += 2;
			string::size_type length = world->messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			Item item(world->messages.substr(pos, length));
			addItemToStash(position, item);
			pos += length;
		}
		/* if there are no items in this stash, erase it */
		if (levels[position.level].stashes[position].items.size() <= 0)
			levels[position.level].stashes.erase(position);
	} else if (world->messages.find(MESSAGE_YOU_SEE_NO_OBJECTS, 0) != string::npos || world->messages.find(MESSAGE_THERE_IS_NOTHING_HERE, 0) != string::npos) {
		/* no items on ground */
		levels[position.level].stashes.erase(position);
	} else if ((pos = world->messages.find(MESSAGE_PICK_UP_WHAT, 0)) != string::npos) {
		/* picking up stuff.
		 * we should clear the stash here too and update it */
		clearStash(position);
		pickup.clear();
		pos = world->messages.find("  ", pos + 1);
		while (pos != string::npos && world->messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = world->messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (world->messages[pos - 2] == '-') {
				Item item(world->messages.substr(pos, length));
				pickup[world->messages[pos - 4]] = item;
				addItemToStash(position, item);
			}
			pos += length;
		}
		/* if there are no items in this stash, erase it */
		if (levels[position.level].stashes[position].items.size() <= 0)
			levels[position.level].stashes.erase(position);
	} else if ((pos = world->messages.find(MESSAGE_DROP_WHICH_ITEMS, 0)) != string::npos) {
		/* dropping items */
		drop.clear();
		pos = world->messages.find("  ", pos + 1);
		while (pos != string::npos && world->messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = world->messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (world->messages[pos - 2] == '-')
				drop[world->messages[pos - 4]] = Item(world->messages.substr(pos, length));
			pos += length;
		}
	} else if (world->messages.find(MESSAGE_NOT_CARRYING_ANYTHING, 0) != string::npos || world->messages.find(MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD, 0) != string::npos) {
		/* our inventory is empty. how did that happen? */
		inventory.clear();
	} else if ((pos = world->messages.find(".  ", 0)) != string::npos) {
		/* when we pick up stuff we only get "  f - a lichen corpse.  " and similar.
		 * we'll need to handle this too somehow.
		 * we're searching for ".  " as we won't get that when we're listing inventory.
		 * also, this won't detect gold, but we might not need to detect that,
		 * well, it's gonna be a bit buggy when picking up gold from stashes */
		/* additionally, we'll assume we're picking up from the stash at this location.
		 * this will also trigger on wishes, but meh, probably not gonna be an issue */
		pos = 0;
		while ((pos = world->messages.find(" - ", pos)) != string::npos) {
			if (pos > 2 && world->messages[pos - 3] == ' ' && world->messages[pos - 2] == ' ') {
				unsigned char key = world->messages[pos - 1];
				pos += 3;
				string::size_type length = world->messages.find(".  ", pos);
				if (length == string::npos)
					break;
				length = length - pos;
				Item item(world->messages.substr(pos, length));
				addItemToInventory(key, item);
				removeItemFromPickup(item);
				removeItemFromStash(position, item);
				pos += length;
			} else {
				/* "Yak - dog food!" mess things up */
				++pos;
			}
		}
	} else if ((pos = world->messages.find(" - ", 0)) != string::npos) {
		/* we probably listed our inventory */
		if (world->cur_page == 1)
			inventory.clear(); // only clear when we're listing 1st page
		while ((pos = world->messages.find(" - ", pos)) != string::npos) {
			if (pos > 2 && world->messages[pos - 3] == ' ' && world->messages[pos - 2] == ' ') {
				unsigned char key = world->messages[pos - 1];
				pos += 3;
				string::size_type length = world->messages.find("  ", pos);
				if (length == string::npos)
					break;
				length = length - pos;
				Item item(world->messages.substr(pos, length));
				addItemToInventory(key, item);
				pos += length;
			}
		}
	}
	/* we'll need to set the on_ground pointer here */
	if (levels[position.level].stashes.find(position) != levels[position.level].stashes.end())
		on_ground = &levels[position.level].stashes[position];
}

void Saiph::removeItemFromPickup(const Item &item) {
	/* we currently don't have a way to tell exactly which item we removed.
	 * we'll have to search and remove the best match */
	if (item.count <= 0)
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from pickup" << endl;
	for (map<unsigned char, Item>::iterator p = pickup.begin(); p != pickup.end(); ++p) {
		if (p->second.name != item.name)
			continue;
		if (p->second.count > item.count) {
			/* we got more than we remove */
			p->second.count -= item.count;
		} else {
			/* removing all we got */
			pickup.erase(p);
		}
		return;
	}
}

void Saiph::setDungeonSymbol(const Point &point, unsigned char symbol) {
	/* since we're gonna track certain symbols we'll use an own method for this */
	if (levels[position.level].dungeonmap[point.row][point.col] == symbol)
		return; // no change
	if (track_symbol[levels[position.level].dungeonmap[point.row][point.col]])
		levels[position.level].symbols[levels[position.level].dungeonmap[point.row][point.col]].erase(point);
	if (track_symbol[symbol])
		levels[position.level].symbols[symbol][point] = -1;
	levels[position.level].dungeonmap[point.row][point.col] = symbol;
}

void Saiph::updateMaps() {
	/* update the various maps */
	for (vector<Point>::iterator c = world->changes.begin(); c != world->changes.end(); ++c) {
		if (world->view[c->row][c->col] == SOLID_ROCK)
			continue; // not interesting (also mess up unlit rooms)
		if (dungeon[(unsigned char) world->view[c->row][c->col]]) {
			/* update the map showing static stuff */
			setDungeonSymbol(*c, world->view[c->row][c->col]);
		} else if (!passable[levels[position.level].dungeonmap[c->row][c->col]]) {
			/* we can't see the floor here, but we believe we can pass this tile.
			 * place an UNKNOWN_TILE here.
			 * the reason we check if stored tile is !passable is because if we don't,
			 * then every tile a monster steps on or drops an item on will become UNKNOWN_TILE,
			 * even if we already know what's beneath the monster/item. */
			setDungeonSymbol(*c, UNKNOWN_TILE);
		}
		/* update items */
		if (item[(unsigned char) world->view[c->row][c->col]]) {
			map<Point, Stash>::iterator s = levels[position.level].stashes.find(*c);
			if (s != levels[position.level].stashes.end()) {
				if (!world->player.hallucinating && (s->second.top_symbol != world->view[c->row][c->col] || s->second.top_color != world->color[c->row][c->col])) {
					/* top symbol/color changed, update */
					s->second.turn_changed = world->player.turn;
					s->second.top_symbol = world->view[c->row][c->col];
					s->second.top_color = world->color[c->row][c->col];
				}
			} else {
				/* new stash */
				levels[position.level].stashes[*c] = Stash(world->player.turn, world->view[c->row][c->col], world->color[c->row][c->col]);
			}
		} else if (world->view[c->row][c->col] == levels[position.level].dungeonmap[c->row][c->col]) {
			/* if there ever was a stash here, it's gone now */
			levels[position.level].stashes.erase(*c);
		}

		/* update monsters */
		if (monster[(unsigned char) world->view[c->row][c->col]]) {
			/* add a monster, or update position of an existing monster */
			unsigned char symbol;
			if (world->color[c->row][c->col] == INVERSE)
				symbol = PET;
			else
				symbol = world->view[c->row][c->col];
			/* set monster on monstermap */
			levels[position.level].monstermap[c->row][c->col] = symbol;
			/* find nearest monster */
			int min_distance = INT_MAX;
			map<Point, Monster>::iterator nearest = levels[position.level].monsters.end();
			for (map<Point, Monster>::iterator m = levels[position.level].monsters.begin(); m != levels[position.level].monsters.end(); ++m) {
				if (m->second.symbol != symbol || m->second.color != world->color[c->row][c->col])
					continue; // not the same monster
				unsigned char old_symbol;
				if (world->color[m->first.row][m->first.col] == INVERSE)
					old_symbol = PET;
				else
					old_symbol = world->view[m->first.row][m->first.col];
				if (m->second.symbol == old_symbol && m->second.color == world->color[m->first.row][m->first.col])
					continue; // this monster already is on its square
				/* see if this monster is closer than the last found monster */
				int distance = max(abs(m->first.row - c->row), abs(m->first.col - c->col));
				if (distance >= min_distance)
					continue;
				/* it is */
				min_distance = distance;
				nearest = m;
			}
			if (nearest != levels[position.level].monsters.end()) {
				/* we know of this monster, move it to new location */
				/* remove monster from monstermap */
				levels[position.level].monstermap[nearest->first.row][nearest->first.col] = ILLEGAL_MONSTER;
				/* update monster */
				levels[position.level].monsters[*c] = nearest->second;
				levels[position.level].monsters.erase(nearest);
			} else {
				/* add monster */
				levels[position.level].monsters[*c] = Monster(symbol, world->color[c->row][c->col]);
			}
		}
	}
	/* remove monsters that seems to be gone
	 * and make monsters we can't see !visible */
	for (map<Point, Monster>::iterator m = levels[position.level].monsters.begin(); m != levels[position.level].monsters.end(); ) {
		unsigned char symbol;
		if (world->color[m->first.row][m->first.col] == INVERSE)
			symbol = PET;
		else
			symbol = world->view[m->first.row][m->first.col];
		/* if we don't see the monster on world->view then it's not visible */
		m->second.visible = (symbol == m->second.symbol && world->color[m->first.row][m->first.col] == m->second.color);
		if (abs(position.row - m->first.row) > 1 || abs(position.col - m->first.col) > 1) {
			/* player is not next to where we last saw the monster */
			++m;
			continue;
		}
		if (symbol == m->second.symbol && world->color[m->first.row][m->first.col] == m->second.color) {
			/* we can still see the monster */
			++m;
			continue;
		}
		/* remove monster from monstermap */
		levels[position.level].monstermap[m->first.row][m->first.col] = ILLEGAL_MONSTER;
		/* remove monster from list */
		levels[position.level].monsters.erase(m++);
	}
	/* update map used for pathing */
	updatePathMap();
}

void Saiph::updatePathMap() {
	/* first reset nextnode pointer, cost & move */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			pathmap[r][c].nextnode = NULL;
			pathmap[r][c].cost = UINT_MAX;
			pathmap[r][c].move = ILLEGAL_MOVE;
		}
	}
	Point from(world->player.row, world->player.col);
	pathing_queue[0] = from;
	pathmap[from.row][from.col].cost = 0;
	pathmap[from.row][from.col].move = REST;
	int curnode = 0;
	int nodes = 1;
	while (curnode < nodes) {
		from = pathing_queue[curnode++];
		/* check northwest node */
		Point to(from.row - 1, from.col - 1);
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_NW;
			pathing_queue[nodes++] = to;
		}
		/* check north node */
		++to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_N;
			pathing_queue[nodes++] = to;
		}
		/* check northeast node */
		++to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_NE;
			pathing_queue[nodes++] = to;
		}
		/* check east node */
		++to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_E;
			pathing_queue[nodes++] = to;
		}
		/* check southeast node */
		++to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_SE;
			pathing_queue[nodes++] = to;
		}
		/* check south node */
		--to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_S;
			pathing_queue[nodes++] = to;
		}
		/* check southwest node */
		--to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_SW;
			pathing_queue[nodes++] = to;
		}
		/* check west node */
		--to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_W;
			pathing_queue[nodes++] = to;
		}
	}
}

bool Saiph::updatePathMapHelper(const Point &to, const Point &from) {
	/* helper method for updatePathMap()
	 * return true if the move is legal and we should path further from this node */
	if (to.row < MAP_ROW_BEGIN || to.row > MAP_ROW_END || to.col < MAP_COL_BEGIN || to.col > MAP_COL_END)
		return false; // outside map
	unsigned char s = levels[position.level].dungeonmap[to.row][to.col];
	if (!passable[s])
		return false;
	unsigned char m = levels[position.level].monstermap[to.row][to.col];
	if (monster[m] && m != PET)
		return false; // can't path through monsters (except pets)
	bool cardinal_move = (to.row == from.row || to.col == from.col);
	if (!cardinal_move) {
		if (s == OPEN_DOOR || levels[position.level].dungeonmap[from.row][from.col] == OPEN_DOOR)
			return false; // diagonally in/out of door
		if (s == UNKNOWN_TILE || levels[position.level].dungeonmap[from.row][from.col] == UNKNOWN_TILE)
			return false; // don't know what tile this is, it may be a door. no diagonal movement
		unsigned char sc1 = levels[position.level].dungeonmap[to.row][from.col];
		unsigned char sc2 = levels[position.level].dungeonmap[from.row][to.col];
		if (!passable[sc1] && !passable[sc2]) {
			/* moving past two corners
			 * while we may pass two corners if we're not carrying too much we'll just ignore this.
			 * it's bound to cause issues */
			if (sc1 != BOULDER && sc2 != BOULDER)
				return false; // neither corner is a boulder, we may not pass
			/*
			else if (position.branch == BRANCH_SOKOBAN)
				return false; // in sokoban we can't pass by boulders diagonally
			*/
		}
		//if (polymorphed_to_grid_bug)
		//	return false;
		//if (rogue_level)
		//	return false; // level that is hard to parse. only allowing cardinal moves makes it easier
	}
	//if (blacklisted_move)
	//	return false;
	//if (s == LAVA && !levitating)
	//	return false;
	//if (s == WATER && (!levitating || !waterwalk))
	//	return false;
	unsigned int newcost = pathmap[from.row][from.col].cost + (cardinal_move ? COST_CARDINAL : COST_DIAGONAL);
	newcost += pathcost[s];
	newcost += pathcost[m];
	if (newcost < pathmap[to.row][to.col].cost) {
		pathmap[to.row][to.col].nextnode = &pathmap[from.row][from.col];
		pathmap[to.row][to.col].cost = newcost;
		return true;
	}
	return false;
}

/* main */
int main() {
	Saiph *saiph = new Saiph(CONNECTION_LOCAL);
	//for (int a = 0; a < 200 && saiph->run(); ++a)
	//	;
	while (saiph->run())
		;
	saiph->debugfile << SAIPH_DEBUG_NAME << "Quitting gracefully" << endl;
	delete saiph;
}
