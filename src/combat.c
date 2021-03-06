/*------------------------------------------------------------------------------
| NuCTex	| combat.c
| Author	| Benjamin A - Nullsrc
| Created	| 29 December, 2015
|-------------------------------------------------------------------------------
| Overview	| Implementation of combat systems. Menu selection for which
|			| creature will be fought is implemented in gamef.c
\-----------------------------------------------------------------------------*/

#include <stdio.h>
#include "combat.h"
#include "io.h"
#include "math.h"


/* combat(Actor*, Actor*) ---
   The combat function is the main loop for the combat system. It calls several
   other functions. The int returned is intended to be used with the game_isQuit
   variable, to simulate player death. It will eventually be phased out in favor
   of a better death system (current thoughts point towards listing all killed
   monsters by moving the player to the dead location) */
int combat(Actor* player, Actor* creature) {
	if(!player || !creature) {
		logError("Expected two actor; received at least one NULL", __func__);
		return;
	}
	printMessage("The monster stands before you menacingly.");
	
	/* This is the main combat loop. It will exit when the player has run, or
	   either the player or monster has died */
	while(player->health > 0 && hasRun == 0 && creature->health > 0) {
		// action integer stores the value returned by fightMenu()
		int action = fightMenu();

		/* This switch statement controls the various actions of combat, and is
		   intended to use action as it's value to check */
		switch(action) {
			case 0:
				break;
			case 1:
				attack(player, creature, 1);
				break;
			case 2:
				printMessage("You rest to regain your energy...");
				break;
			case 3:
				run(player, creature);
				break;
			default:logError("Unexpected integer passed to switch statement", __func__);
		}
		
		/* This function controls monster actions, and is a placeholder until
		   a more sophisticated system that utilizes AI flags is in place */
		monsterAct(arng(10), player, creature); 
	}

	// These conditionals handle the after-battle cleanup
	if(player->health <= 0) {
		printMessage("You have died.");

		// return value of 1 sets game_isQuit to 1
		return 1;
	}
	else if(hasRun != 0) {
		printMessage("You manage to flee the fight.");
		hasRun = 0;
		return 0;
	}
	else if(creature->health <=0) {
		printMessage("You slay the monster.");

		// Move the creature to the location reserved for dead things
		creature->actorPos = &LIndex.loc[0];
		return 0;
	}
	else {
		return 0;
	}
}

/* fightMenu() ---
   Returns integers based on the user input */
int fightMenu() {
	printMessageC("=============================", ANSI_RED);
	printMessageC("1: Attack | 2: Rest | 3: Flee", ANSI_YELLOW);
	printMessageC("=============================", ANSI_RED);
	char* choice = getInput();
	if(checkOne(choice, "1") == 0) {
		return 1;
	}
	else if(checkOne(choice, "2") == 0) {
		return 2;
	}
	else if(checkOne(choice, "3") == 0) {
		return 3;
	}
	else {
		return 0;
	}
}

/* monsterAct(int, Actor*, Actor*) ---
   Controls the current monster AI (what little there is). Player is passed to
   allow the use of an attack call from the monster */
void monsterAct(int number, Actor* player, Actor* creature) {
	// number should be produced from an arng call until the system is rewritten
	if(number == 0) {
		printMessage("The monster is still, and appears to be taking a rest");
	}
	else {
		attack(creature, player, 0);
	}
}

/* attack(Actor*, Actor*, int) ---
   Call a damage calculation given two actor's stats. The int should only be 1
   when the player is attacking */
void attack(Actor* attacker, Actor* defender, int isPlayerAttacking) {
	int damage;
	/* If the attacker does not have a weapon, he only does damage based on his
	   strength */
	if(!attacker->eqp.weapon) {
		damage = calcDamage(attacker->strength);
	}
	/* If the attacker has a weapon, the weapon's strength is added to his own
	   during the calcDamage() call */
	else {
		damage = calcDamage(attacker->strength + attacker->eqp.weapon->strength);
		if(attacker->eqp.weapon->flags[0]) {
			if(itemHasFlag(attacker->eqp.weapon, &sharp)) {
				damage = damage * 10;
			}
		}
	}

	// Subtract the defender's defense from the damage, if there is any
	damage -= getAggregateDefense(defender->eqp);
	// Zero out damage if it goes negative
	if(damage < 0) {
		damage = 0;
	}
	defender->health -= damage;
	/* Print the attack message a certain way if the player is attacking, and
	   another way if a monster is attacking */
	if(isPlayerAttacking == 1) {
		printDamage(damage, defender->name);
	}
	else {
		printf("The %s hits you and does %i damage!\n", attacker->name, damage);
	}
}

/* run(Actor*, Actor*) ---
   Call a run calculation given two actor's stats. */
void run(Actor* escaping, Actor* chasing) {
	hasRun = runAway(escaping->agility, chasing->agility);
}
