# copy_move
This is a verbose clone of the GNU coreutils cp and mv.
It copies or moves sources to a destination.
Download and compile with (needs gcc):
```
git clone https://github.com/wharvex/copy_move.git
cd copy_move
make
```

Then you can run as copy or move:
```
./copy source1 [source2 ...] destination
./move source1 [source2 ...] destination
```

TODO: Improve documentation, refactor repetitive code.
