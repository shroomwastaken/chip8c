# chip8c
chip8 emulator written in c

# usage
only works on linux right now
```./chip8c <rom file name>```

this emulator runs at an *emulated* 60FPS
because computers are very fast now if i didnt cap the framerate it wouldve been impossible to play any games on this thing, everything would be going too fast (and it would take up 35% of your cpu)

# keys

the original chip8 keyboard
<table>
    <tbody>
        <tr>
            <td>1</td>
            <td>2</td>
            <td>3</td>
            <td>C</td>
        </tr>
        <tr>
            <td>4</td>
            <td>5</td>
            <td>6</td>
            <td>D</td>
        </tr>
        <tr>
            <td>7</td>
            <td>8</td>
            <td>9</td>
            <td>E</td>
        </tr>
        <tr>
            <td>A</td>
            <td>0</td>
            <td>B</td>
            <td>F</td>
        </tr>
    </tbody>
</table>

maps to a normal keyboard like this

<table>
    <tbody>
        <tr>
            <td>1</td>
            <td>2</td>
            <td>3</td>
            <td>4</td>
        </tr>
        <tr>
            <td>Q</td>
            <td>W</td>
            <td>E</td>
            <td>R</td>
        </tr>
        <tr>
            <td>A</td>
            <td>S</td>
            <td>D</td>
            <td>F</td>
        </tr>
        <tr>
            <td>Z</td>
            <td>X</td>
            <td>C</td>
            <td>V</td>
        </tr>
    </tbody>
</table>

# why
emulation is really cool and i wanted to try out coding in c

# changelog
**v0.1.0**
- all opcodes, timers work, input works, no sound yet
