import sys
import rclpy


def main():
    print('Hi from eros2py_package.')
    rclpy.init(args=sys.argv)
    node = rclpy.create_node('asdf')

    node.get_logger().info('Created node')

if __name__ == '__main__':
    main()
