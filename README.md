# Shogi Game in C

This is a simple game written in C which simulates the Japanese Shogi chess.

The algorithm is primarily implemented using stacks to allow reverts and replays. There is another version available in another branch which uses link-lists.

## Implemented Features

- Piece movements
- Movement validation
- Piece "eating"
- Reverting steps
- Game timer
- Kifu (game log) export
- Replay of matches

## Excluded Features

- Promotion of pieces
- Use of "eaten" pieces

## Tools

- C Language
- Libev event library

## Dependencies
If you wish to compile the program by yourself, you must have the `libev` library installed on your device.

On Ubuntu, it can be installed using:
```bash
sudo apt install -y libev-dev
```

## Compiling
The program should be compiled using the following command:
```bash
$ gcc -o shogi.out main.c -lm replay.c -lm shogi.c -lm -lev shogilib.c -lm kifu.c -lm
```

## Running the game
There are two modes available —— match and replay.

To start a new game, use the following command:
```bash
$ ./shogi.out -n -s <KIFU_FILE_NAME>
```

To replay a saved match, use the following command:
```bash
$ ./shogi.out -l <KIFU_FILE_NAME>
```

## Playing the game
To move a piece, type:
```bash
> m <OLD_X> <OLD_Y>
```
hit `Enter` and type in the target position:
```bash
> <NEW_X> <NEW_Y>
```

You may save the current kifu using the following command:
```bash
> s
```
A new save file will be generated automatically. If the file already exists, it will be overwritten.


## Replaying a saved match
You may replay forward and backwards when in replay mode. `p` replays the next step while `b` replays the previous step.

You may quit the replay mode using the `q` command.
