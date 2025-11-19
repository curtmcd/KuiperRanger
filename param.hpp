#ifndef param_hpp
#define param_hpp

#include "type.hpp"

// Note: all pixel distances (including in shape and font character
// definitions and in pixel/sec velocities) assume a nominal display
// size of 1280 x 960. The Plot module takes care of dynamic rescaling
// to the actual display size.

#define ASPECT_X	4
#define ASPECT_Y	3
#define NOMINAL_WIDTH	1280.0		// 1280 x 960
#define NOMINAL_HEIGHT	(NOMINAL_WIDTH * ASPECT_Y / ASPECT_X)

// Game
#define TITLE		"Kuiper Ranger"
#define VERSION		"2.5"
#define ENVVAR_WIDTH	"PLOT_WIDTH"
#define MAXPLAYERS	3
#define MARGINCOLOR	"grey5"		// Used for full-screen mode only

// Ship
#define SHIPSHOTSPEED	240.0		// Missile speed (pix/sec)
#define SHIPSHOTDUR	1.50		// Duration of missiles (sec)
#define MISSILELIMIT	6
#define HYPERSPACETIME	1.5		// Duration of hyperspace (sec)
#define SHIPROTSPEED	225.0		// Rotation speed (deg/sec)
#define STARTLIVES	3		// Less than MAXEXTRA
#define MAXLIVES	5		// Maximum number of extra ships
#define EXTRASHIPPOINTS	10000		// Score to get an extra ship
#define EXTRAROTSPEED	180.0		// Extra ship rotation speed (deg/sec)
#define SHIPACCEL	500.0		// Ship acceleration (pix/sec^2)
#define SHIPSPEEDMAX	900.0		// Maximum ship speed (pix/sec)
#define SHIPFRICTION	PERCENT(20)	// Ship speed reduction factor (%/sec)
#define SHIPFLAMEBLINK	0.2		// Blink period (sec)
#define SHIPFLAMEDUTY	PERCENT(75)	// Blink duty cycle
#define SHIPDEATHTIME	4.0		// Wait time before next wave (sec)
#define SHIPSHARDS	15		// Number of shards in ship explosion

// Ghost (start period during which player is blinking)
#define GHOSTBLINK	0.55		// Blink period (sec)
#define GHOSTDUTY	PERCENT(66)	// Blink duty cycle

// Alien
#define ALIENSPEED	60.0		// Alien speed (pix/sec)
#define ALIENSPEEDINC	30.0		// Add'l speed per smaller alien (pix/sec)
#define ALIENSHOTSPEED	240.0		// Missile speed (pix/sec)
#define ALIENSHOTDUR	1.5		// Missile duration (sec)
#define ALIENWANDERING	3.0		// Mean time between changes
                                        //  to alien vertical direction
#define ALIENSPAWNTIME	10.0		// Average time between aliens
#define ALIENSIZES 	2
#define ALIENSHARDS	16		// Number of shards in alien explosion

// Rocks
#define INITWAVESIZE	4		// Initial # of large rocks in a game
#define INCWAVESIZE	1		// Large rocks added in next wave
#define WAVEDELAY	3.5		// Delay before new set of rocks (sec)
#define ROCKBASESIZE	PERCENT(200)	// Arbitrary/relative
#define ROCKSIZES	3
#define SPLITINTO	2
#define MAXROCKVEL	150.0		// Max velocity for new rocks (pix/sec)
#define MAXROCKROT	90.0		// Max angular velocity (deg/sec)
#define ROCKMARGIN	PERCENT(20)	// Window margin in which rocks may appear

// Explosions
#define SHARDVEL	3.0		// Velocity of shards (pix/sec)
#define SHARDSIZE	PERCENT(15)	// Size of shards compared to object
#define SHARDDECAY	PERCENT(90)	// Shard decay factor (%/sec)
#define SHARDMAXROT	900.0		// Shard max angular velocity (deg/sec)

// Scoring:
//  Rock sizes are 0 (biggest), 1, 2, ...
//  Score for rocks is (MULROCKSCORE * size + BASEROCKSCORE)
//  Alien sizes are 0 (large), 1, ...
//  Score for aliens is (MULALIENSCORE * size + BASEALIENSCORE)
#define BASEROCKSCORE	10
#define MULROCKSCORE	20
#define BASEALIENSCORE	100
#define MULALIENSCORE	200
#define ASCORETIMEOUT	2.0		// Duration for showing score (sec)
#define SCOREDIGITS	6

// Screen positioning
#define SCOREBARH	56		// height of score area at top
#define EXTRAX		100		// x coord of extra-ship display
#define SCOREX		400		// x coord of current score
#define HIGHX		800		// x coord of high score

// High score
#define NICKMAXLEN	8		// max chars in high score nicknames
#define HIGHROWS	5		// # rows of high scores
#define HIGHCOLS	3		// # cols of high scores
#define HIGHTIMEOUT	(4 * WEEK_SEC)	// # sec before other high scores time out
#define HIGHPERMANENT	(HIGHCOLS * 2)	// # highest scores that never time out

// Font
#define ITALICSLOPE	PERCENT(30)

// Sound
#define DEETINITPERIOD	1.0		// Initial 'deet' interval (sec)
#define DEETMINPERIOD	0.25		// Fastest 'deet' interval can get (sec)
#define DEETSPEEDUP	PERCENT(85)	// 'deet' period multiplier per wave

#define SPEAKERFLASH	2.0		// Duration speaker icon shows (sec)

#endif // !param_hpp
