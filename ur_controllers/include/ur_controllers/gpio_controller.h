// this is for emacs file handling -*- mode: c++; indent-tabs-mode: nil -*-

// -- BEGIN LICENSE BLOCK ----------------------------------------------
// Copyright (c) 2021 PickNik LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// -- END LICENSE BLOCK ------------------------------------------------

//----------------------------------------------------------------------
/*!\file
 *
 * \author  Lovro Ivanov lovro.ivanov@gmail.com
 * \date    2021-02-17
 *
 */
//----------------------------------------------------------------------
#ifndef UR_ROBOT_DRIVER_TOOLIOCONTROLLER_H
#define UR_ROBOT_DRIVER_TOOLIOCONTROLLER_H

#include <controller_interface/controller_interface.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <ur_msgs/msg/io_states.hpp>
#include <ur_msgs/msg/tool_data_msg.hpp>
#include <ur_dashboard_msgs/msg/robot_mode.hpp>
#include <ur_dashboard_msgs/msg/safety_mode.hpp>

#include <ur_msgs/srv/set_io.hpp>
#include <ur_msgs/srv/set_speed_slider_fraction.hpp>

namespace ur_controllers
{
class GPIOController : public controller_interface::ControllerInterface
{
public:
  controller_interface::InterfaceConfiguration command_interface_configuration() const override;

  controller_interface::InterfaceConfiguration state_interface_configuration() const override;

  controller_interface::return_type init(const std::string& controller_name) override;

  controller_interface::return_type update() override;

  CallbackReturn on_configure(const rclcpp_lifecycle::State& previous_state) override;

  CallbackReturn on_activate(const rclcpp_lifecycle::State& previous_state) override;

  CallbackReturn on_deactivate(const rclcpp_lifecycle::State& previous_state) override;

private:
  bool setIO(ur_msgs::srv::SetIO::Request::SharedPtr req, ur_msgs::srv::SetIO::Response::SharedPtr resp);

  bool setSpeedSlider(ur_msgs::srv::SetSpeedSliderFraction::Request::SharedPtr req,
                      ur_msgs::srv::SetSpeedSliderFraction::Response::SharedPtr resp);

protected:
  void initMsgs();

  bool first_pass_;

  // internal commands
  std::array<double, 18> standard_digital_output_cmd_;
  std::array<double, 18> standard_analog_output_cmd_;
  double speed_scaling_factor_cmd_;

  // services
  rclcpp::Service<ur_msgs::srv::SetSpeedSliderFraction>::SharedPtr set_speed_slider_srv_;
  rclcpp::Service<ur_msgs::srv::SetIO>::SharedPtr set_io_srv_;

  std::shared_ptr<rclcpp::Publisher<ur_msgs::msg::IOStates>> io_pub_;
  std::shared_ptr<rclcpp::Publisher<ur_msgs::msg::ToolDataMsg>> tool_data_pub_;
  std::shared_ptr<rclcpp::Publisher<ur_dashboard_msgs::msg::RobotMode>> robot_mode_pub_;
  std::shared_ptr<rclcpp::Publisher<ur_dashboard_msgs::msg::SafetyMode>> safety_mode_pub_;

  ur_msgs::msg::IOStates io_msg_;
  ur_msgs::msg::IOStates tool_data_msg_;
  ur_msgs::msg::IOStates robot_mode_msg_;
  ur_msgs::msg::IOStates safety_mode_msg_;

  // TODO publishers to add: program_state_pub_, tcp_pose_pub_
  // TODO subscribers to add: script_command_sub_
  // TODO service servers to add: resend_robot_program_srv_, deactivate_srv_, set_payload_srv_, tare_sensor_srv_
};
}  // namespace ur_controllers

#endif  // UR_ROBOT_DRIVER_TOOLIOCONTROLLER_H
