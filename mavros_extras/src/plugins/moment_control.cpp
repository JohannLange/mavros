/**
 * @brief MomentControls plugin
 * @file moment_controls.cpp
 * @author johannlange@yahoo.de>
 *
 * @addtogroup plugin
 * @{
 */
/*
 * Copyright 2019 Johann Lange.
 *
 * This file is part of the mavros package and subject to the license terms
 * in the top-level LICENSE file of the mavros repository.
 * https://github.com/mavlink/mavros/tree/master/LICENSE.md
 */

#include <mavros/mavros_plugin.h>

#include <mavros_msgs/MomentControl.h>

namespace mavros {
namespace extra_plugins {
/**
 * @brief Manual Control plugin
 */
class MomentControlPlugin : public plugin::PluginBase {
public:
	MomentControlPlugin() : PluginBase(),
		moment_control_nh("~manual_control")
	{ }

	void initialize(UAS &uas_)
	{
		PluginBase::initialize(uas_);

		send_sub = moment_control_nh.subscribe("send_moments", 1, &MomentControlPlugin::send_cb, this);
	}

	Subscriptions get_subscriptions() {
		return { /* RX not defined */ };
	}

private:
	ros::NodeHandle moment_control_nh;

	ros::Subscriber send_sub;


	/* -*- callbacks -*- */

	void send_cb(const mavros_msgs::MomentControl::ConstPtr req)
	{
		mavlink::common::msg::MOMENT_CONTROL msg;
		//msg.target = m_uas->get_tgt_system();

		msg.roll    = req->roll;
		msg.pitch   = req->pitch;
		msg.yaw     = req->yaw;
		msg.thrust  = req->thrust;

		UAS_FCU(m_uas)->send_message_ignore_drop(msg);
	}
};
}	// namespace std_plugins
}	// namespace mavros

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::MomentControlPlugin, mavros::plugin::PluginBase)
