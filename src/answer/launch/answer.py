from launch import LaunchDescription
import launch_ros.actions

def generate_launch_description():
    return LaunchDescription([
        launch_ros.actions.Node(
            namespace= "my_ws", package='answer', executable='my_node', output='screen'),
        launch_ros.actions.Node(
            namespace= "my_ws", package='homework', executable='homework_node', output='screen'),

    ])