import serial, sys, argparse
from time import sleep
import colors as col

signals = [
    ['s_p', col.Red],
    ['s_i', col.Grn],
    ['s_d', col.Blu],
    ['servo', col.BIPur],
    ['motor', col.Cya],
    ['dist', col.Wht],
#    ['d2', col.Wht],
#    ['d3', col.Wht],
    ['vbat', col.Yel],
]

# Package size, not including key
package_size = 7
key_MSB = b'\xaa'
key_LSB = b'\xbb'

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

    # Draw data
    outBuf = [' ']*256

    # Key checking
    while s.read(1) != key_MSB:
      print("First key stage failed: ")
      pass

    if s.read(1) != key_LSB:
      print("Second key stage failed")
      continue

    data = s.read(package_size)

    for i, x in enumerate(data):
      outBuf[x] = signals[i][1] + 'O' + col.Off

    sys.stdout.write('|' + ''.join(outBuf) + '|\n')
    sys.stdout.flush()

    # Draw legends
    outBuf = [' ']*(256+20)

    for i, x in enumerate(data):
      for j, c in enumerate(signals[i][0]):
        outBuf[x+j] = signals[i][1] + c + col.Off

    sys.stdout.write(' ' + ''.join(outBuf) + '\r')


# call main
if __name__ == '__main__':
  main()
