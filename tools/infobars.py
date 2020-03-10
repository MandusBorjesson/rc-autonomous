import serial, sys, argparse, curses, time
from collections import namedtuple

signal = namedtuple('signal', ['name', 'type', 'offset'], defaults=('','',0))
signals = [
    signal('dist', 'u16'),
    signal('err', 's16'),
    signal('s_p', 's16'),
    signal('s_i', 's16'),
    signal('s_d', 's16'),
    signal('servo', 's8'),
    signal('motor', 's8'),
]

symbol_list = ' _.,;:-+*/=()[]'

package_size = 0

# Width of name and value fields
name_width = 8
val_width = 4

key_MSB = b'\xaa'
key_LSB = b'\xbb'

def send_command(port, command):
  command = command.replace(' ', '')
  command = command.split(';')

  for com in command:
    port.write((com + '\x00').lower().encode('utf-8'))
    time.sleep(0.1)

def makeSlider(val, type, max_sz):
      slider = [' ']*(max_sz)
      zero_position = 0

      if type is 's8':
        val = val - 256 if val > 127 else val
        index = ((val + 128)*max_sz)//256
        zero_position = max_sz//2
      elif type is 'u8':
        index = (val*max_sz)//256
      elif type is 's16':
        val = val - 65536 if val > 32767 else val
        index = ((val + 32767)*max_sz)//65536
        zero_position = max_sz//2
      elif type is 'u16':
        index = (val*max_sz)//65536
      else:
        index = 0
        val = 0

      for i in range(min(index, zero_position), max(index, zero_position)):
        slider[i] = '-'

      slider[index] = '#'
      slider[zero_position] = '0'

      return [''.join(slider), val]

def draw_bars(stdscr):
  # create parser
  parser = argparse.ArgumentParser(description='Command line serial plotter')

  # add expected arguments
  parser.add_argument('--port', dest='port', required=True)

  # parse args
  args = parser.parse_args()

  ser = serial.Serial(args.port, 9600, timeout=1)

  cmd_hist = ['', '']
  cmd = ''
  cmd_cntr = 0

  stdscr.nodelay(True)
  stdscr.clear()
  stdscr.refresh()

  while 1:
    # Key checking
    while ser.read(1) != key_MSB:
      pass

    if ser.read(1) != key_LSB:
      continue

    data = ser.read(package_size)

    stdscr.clear()
    height, width = stdscr.getmaxyx()

    # Build up each row as a string
    # | Name     |       <-- Width -->         |  Val |
    row_cntr = 0

    for sig in signals:
      value = 0
      if '16' in sig.type:
        value = data[sig.offset+1] * 256 + data[sig.offset]
      else:
        value = data[sig.offset]

      name = sig.name.ljust(name_width)
      slider_sz = width - name_width - val_width - 8
      slider, val = makeSlider(value, sig.type, slider_sz)
      row = "| {} |{}| {}\n".format(name, slider, str(val).rjust(val_width))
      stdscr.addstr(row_cntr, 0, row)
      row_cntr = row_cntr + 1

    stdscr.addstr(row_cntr, 0, cmd)
    stdscr.refresh()

    # Keypress handling
    try:
      key = stdscr.getkey()
      if key in ('KEY_BACKSPACE', '\b', '\x7f'):
        cmd = cmd[:-1]
      elif key in ('KEY_ENTER', '\x0a'):
        send_command(ser, cmd)
        if cmd_hist[1] is not cmd:
          cmd_hist.insert(1, cmd)
        cmd_cntr = 0
        cmd = ""
      elif key == 'KEY_UP':
        if cmd_cntr < len(cmd_hist):
          cmd_cntr = cmd_cntr + 1
        cmd = cmd_hist[cmd_cntr]
      elif key == 'KEY_DOWN':
        if cmd_cntr > 1:
          cmd_cntr = cmd_cntr - 1
        cmd = cmd_hist[cmd_cntr]
      elif key.isalnum() or key in symbol_list:
        cmd = cmd + key
    except:
      pass

# main() function
def main():
  curses.wrapper(draw_bars)

# call main
if __name__ == '__main__':
  offs = 0
  for i in range(len(signals)):
    signals[i] = signals[i]._replace(offset = offs)
    if '16' in signals[i].type:
      offs = offs + 2
    else:
      offs = offs + 1

  # Package size, not including key
  package_size = offs

  main()
