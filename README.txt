README.txt for Bug Squish

by Bill Kendrick
bill@newbreedsoftware.com
http://www.newbreedsoftware.com/bugsquish/

Version 0.0.6

April 20, 2002


DESCRIPTION
-----------
  "Bug Squish" is an action game not unlike light gun arcade games, but
  played with a mouse.  It's loosely based on a Pangea Software's MacOS game
  "Blood Suckers," by Brian Greenstone.


STORY
-----
  Bugs are trying to suck blood out of your arm!  Squish them with with
  your fly swatter before you run out of blood.


DOCUMENTATION
-------------
  Important documentation for "Bug Squish" is contained in multiple files.
  Please see them:

    AUTHORS.txt    - Credits for who made this game.
    CHANGES.txt    - Changes since the previous versions of "Bug Squish"
    COPYING.txt    - The GNU Public License, which "Bug Squish" is under.
    INSTALL.txt    - Instructions on requirements, compiling and installing.
    README.txt     - (This file.)  Game story, usage, rules and controls.
    TODO.txt       - A wish-list for this version of "Bug Squish"


IMPORTANT NOTE!!!
-----------------
  This game requires a lot of mousing.  Please play responsibly to ensure
  you do not damage your wrist!  Use the game's Pause function to temporarily
  stop your game so you may take breaks.


RUNNING THE GAME
----------------
  Just type "./bugsquish" to get started.

  The program accepts some options:

    --disable-sound     - If sound support was compiled in, this will
                          disable it for this session of the game.

    --fullscreen        - Run in fullscreen mode.

    --help              - Display a help message summarizing command-line
                          options, copyright, and game controls.

    --usage             - Display a brief message summarizing command-line
                          options.


TITLE SCREEN
------------
  At the title screen, you can begin a game, or quit the program.

    SPACE, RETURN or MOUSE-CLICK
      Begin a new game

    ESCAPE or WINDOW-MANAGER-CLOSE
      Quit Bug Squish

  At the lower right corner of the title screen, the final score and wave
  from the last game played are displayed.  Also, the high score and wave
  are displayed.


SCREEN LAYOUT
-------------
    +--------------------+
    |10         x        |
    |8    O        *     |
    |6    x   *          |
    |4       x        #  |
    |2                 \ |
    |XXXXXXXXXXXXXXXXXXXX|
    |XXXXXXXXXXXXXXXXXXXX|
    |Score           Wave|
    +--------------------+

  Status
  ------
    On the far left of the screen is a gauge showing how much blood you
    have left.

    Your current score is displayed at the lower left, in a box labelled
    "Score:".

    The level which you're currently on is displayed at the lower right,
    in a box labelled "Wave:".

  Arm
  ---
    Your arm is shown across the bottom of the screen.  The less blood you
    have, the less healthy (less red) your arm appears.

  Bugs
  ----
    There are a number of different types of bugs.  They appear at the top
    of the screen, and fly down towards your arm.  Different types of bugs
    move in different ways.

  Fly Swatter
  -----------
    Your mouse pointer appears as a small fly swatter shaped image.

  Squished Bugs
  -------------
    When you kill a bug (by placing the fly swatter (mouse pointer) over it
    and clicking), it is squished.  The squished bug remains on the screen
    until the next level.

  Extras
  ------
    Occasionally, a floating ball will appear which contains an "Extra."
    They appear from any side of the screen and fly across and off the
    edge of the screen.  Click it while it's on the screen to get the
    extra bonus it contains.


PLAYING THE GAME
----------------
  Blood
  -----
    You begin with a full gauge of blood.  When all of the blood has been
    sucked out by bugs, the game ends.

  Bugs
  ----
    Bugs appear at the top of the screen and fly down towards your arm,
    where they land to suck your blood.  Some bugs fly downwards very quickly.
    Different waves of the game introduce different kinds of bugs, which
    move around in different ways.

  Squishing Bugs
  --------------
    Use your mouse to position the fly-swatter-shaped mouse pointer over a
    bug, then click any mouse button to squish it.  The higher on the screen
    the bug is, the more points you receive.

  Being Bitten
  ------------
    When a bug lands on your arm, you hear yourself yell "ouch!", and the
    bug begins drawing blood.  Squish it quickly to lose the least amount
    of blood!

  Extras
  ------
    "Extra" bonuses occasionally appear on the screen.  Click them to get
    the bonus, if it applies:

      Extra Blood
      -----------
        Click the red "Extra Blood" bonus to get a little extra blood
        added to your blood gauge, if it isn't full.

        When you click it, it will be squished on the screen, and you'll
        hear an "ahh!" sound.  (Note: If your gauge is full, you obviously
        won't get any extra blood.  You'll instead hear a "glug" sound.)

      Multiplier
      ----------
        Click the blue "x 2" bonus to turn on a score multiplier.
        The multiplier is used when you complete a wave, during the
        "Blood Bonus." (See below.)

        When you click it, it will be squished to the screen, and you'll
        hear a "cha-ching" (cash register) sound.  (Note: If you already got
        a multiplier during the current wave, you'll instead hear a "glug"
        sound.)

  Blood Bonus
  -----------
    Once you've squished all of the bugs that appear during a wave, you
    move onto the next wave.  Before you begin the next wave, you get bonus
    points for the blood you have remaining.

    A "BONUS" box appears in the middle of the screen, and the blood in the
    gauge begins emptying.  You'll hear "glug-glug-glug..." as the blood
    drains out.

    The amount of score you will receive for the blood is tallied up in the
    "BONUS" box.

    Note: If you squished a Multiplier Extra ("x 2") during the wave, the
    blood bonus will be doubled.

    Once all of the blood is converted into bonus points, the bonus box
    goes away, and the bonus points are added to your score.  You will hear
    a "cha-ching" (cash register) sound.  Now, the next wave begins...

  Waves
  -----
    There are 32 waves in Bug Squish.  (Once you beat the 32nd wave, you
    remain on it for the remainder of your game.)

    Successive waves have:

      * More bugs appear when the wave begins (up to a limit)
      * Faster moving bugs
      * More kinds of bugs
      * More bugs, total (before the wave is completed)

  Pause
  -----
    To pause the game, press the SPACE key.  The word "PAUSED" will appear
    on the screen.  To resume, press SPACE again.

  Surrendering
  ------------
    To quit your current game, press the ESCAPE key.  You'll return to the
    title screen.

  Quitting
  --------
    To quit the program entirely, you can either press ESCAPE at the title
    screen, or use your window manager's "Close Window" option (typically an
    "X"-shaped button on the window's title bar).


SCORING
-------
  Squishing Bugs
  --------------
    The number of points you receive for squishing a bug depends on how
    far down the screen the bug has flown.  The higher on the screen the
    bug is, the more points you will receive.

  Blood Bonus
  -----------
    At the end of each wave, bonus points are given based on how much blood
    you have remaining.  If you squished a Multiplier Extra ("x 2") during
    the wave, you receive double points.

  Bonus Blood
  -----------
    Every 10,000 points you receive some bonus blood.  (Just as if you
    clicked an "Extra Blood" extra.)  You obviously won't receive any extra
    blood if your gauge is full.

    When you receive Bonus Blood at a 10,000 point mark, you'll hear the
    "Ahh!" sound.


GAME OVER
---------
  When you run out of blood, the game ends.  You hear yourself yell "Ugh!",
  and any bugs remaining on the screen fly away towards the top.

  The game returns to the title screen, where you can see your final score
  and wave number.


THE END
-------
  Thanks for trying out Bug Squish.  Feel free to share it with your friends!

  If you like Bug Squish, please rate and review it at the Linux Game Tome:

    http://www.happypenguin.org/show?bug%20squish


  Thanks!

  Bill Kendrick  <bill@newbreedsoftware.com>
