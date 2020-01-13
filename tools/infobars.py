import serial, sys, argparse, curses
import colors as col

signals = [
    ['s_p', col.Red, 's8'],
    ['s_i', col.Grn, 's8'],
    ['s_d', col.Blu, 's8'],
    ['servo', col.BIPur, 's8'],
    ['motor', col.Cya, 's8'],
    ['dist', col.Wht, 'u8'],
    ['vbat', col.Yel, 'u8'],
]

# Package size, not including key
package_size = 7

# Width of name and value fields
name_width = 8
val_width = 4

key_MSB = b'\xaa'
key_LSB = b'\xbb'

def makeSlider(val, type, max_sz):
      slider = [' ']*(max_sz)

      if type is 's8':
        val = val - 256 if val > 127 else val
        index = ((val + 128)*max_sz)//256
        slider[max_sz//2] = '0'
      elif type is 'u8':
        index = (val*max_sz)//256
        slider[0] = '0'
      else:
        index = 0
        val = 0

      slider[index] = "#"

      return [''.join(slider), val]

def draw_bars(stdscr):
  # create parser
  parser = argparse.ArgumentParser(description="Command line serial plotter")

  # add expected arguments
  parser.add_argument('--port', dest='port', required=True)

  # parse args
  args = parser.parse_args()

  s = serial.Serial(args.port, 9600, timeout=1)

  stdscr.clear()
  stdscr.refresh()

  while 1:
    # Key checking
    while s.read(1) != key_MSB:
      pass

    if s.read(1) != key_LSB:
      continue

    data = s.read(package_size)

    stdscr.clear()
    height, width = stdscr.getmaxyx()

    # Build up each row as a string
    # | Name     |       <-- Width -->         |  Val |
    outBuf = ''

    for i, x in enumerate(data):
      name = signals[i][0].ljust(name_width)
      slider_sz = width - name_width - val_width - 8
      slider, val = makeSlider(x, signals[i][2], slider_sz)
      row = "| {} |{}| {}\n".format(name, slider, str(val).rjust(val_width))
      stdscr.addstr(i, 0, row)

    stdscr.refresh()

# main() function
def main():
  curses.wrapper(draw_bars)

# call main
if __name__ == '__main__':
  main()
