"""
ldr.py
Display analog data from Arduino using Python (matplotlib)
Author: Mahesh Venkitachalam
Website: electronut.in
"""

import serial, sys, argparse
from time import sleep
import colors as col

signals = [
    ['s_p', col.Red],
    ['s_i', col.Grn],
    ['s_d', col.Blu],
    ['servo', col.BIPur],
    ['motor', col.Cya],
    ['d1', col.Wht],
    ['d2', col.Wht],
    ['d3', col.Wht],
    ['vbat', col.Blk],
]


# main() function
def main():
  # create parser
  parser = argparse.ArgumentParser(description="Command line serial plotter")

  # add expected arguments
  parser.add_argument('--port', dest='port', required=True)

  parser.add_argument('-s', '--start', action='store_true', required=False)

  parser.add_argument('--k_p', dest='k_p', required=False)
  parser.add_argument('--k_i', dest='k_i', required=False)
  parser.add_argument('--k_d', dest='k_d', required=False)
  parser.add_argument('--max_speed', dest='max_speed', required=False)

  # parse args
  args = parser.parse_args()

  s = serial.Serial(args.port, 9600, timeout=1)

  if args.start:
    s.write(str.encode('start'))
    print("START!")


  while 1:
    outBuf = [' ']*256

    data = s.read(4)

    for i, x in enumerate(data):
      outBuf[x] = signals[i][1] + 'O' + col.Off


#    print('\r' + '|' + ''.join(outBuf) + '|')
    sys.stdout.write('|' + ''.join(outBuf) + '|\n')
    sys.stdout.flush()

    sys.stdout.write('| HEJ!\r')



#sys.stdout.write('\r' + '|%s%s%s%s| %d%%' % (unitColor, '\033[7m' + ' '*incre + ' \033[27m', endColor, ' '*(50-incre), 2*incre)) if i != count — 1 else sys.stdout.write('\r' + '|%s%s%s| %d%%' % (unitColor, '\033[7m' + ' '*20 + 'COMPLETE!' + ' '*21 + ' \033[27m’, endColor, 100))
# call main
if __name__ == '__main__':
  main()