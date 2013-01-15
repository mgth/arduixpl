/*
  ArduixPL - xPL for arduino
  Copyright (c) 2012 Mathieu GRENET.  All right reserved.

  This file is part of ArduixPL.

    ArduixPL is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ArduixPL is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ArduixPL.  If not, see <http://www.gnu.org/licenses/>.

	  Modified Dec 23, 2012 by Mathieu GRENET
*/
	C(xpl)
	C(stat)
	C(cmnd)
	C(trig)

	C(hop)
	C(source)
	C(target)

	C(test)
	C(message)

	CF(_vendorid,"arduixpl")
	CF(_deviceid,"dummy")
	CF(_instanceid,"default")

	// sensor
	C(sensor)
	C(device)
	C(type)
	C(name)
	C(current)
	C(lowest)
	C(highest)
	C(units)

	C(inches)
	C(hours)
	C(volts)

	C(battery)
	C(count)
//	C(current)
	C(direction)
	C(distance)
	C(energy)
	C(fan)
	C(generic)
	C(humidity)
	C(input)
	C(output)
	C(power)
	C(pressure)
	C(setpoint)
	C(speed)
	C(temp)
	C(uv)
	C(voltage)
	C(volume)
	C(weight)

	// adapter
	C(adapter)
	CF(mac_address,"mac-address")
	CF(ip_address,"ip-address")
	CF(ip_mask,"ip-mask")
	C(dhcp)

	// hbeat
	C(hbeat)
	C(basic)
	C(end)
	C(group)
	C(filter)



	C(response)
	C(newconf)
	C(config)
	C(reconf)
	C(option)
	C(interval)
//	C(current)
	C(state)
	C(status)

	//booleans
	C(on)
	C(off)
	CF(_true,"true")
	CF(_false,"false")
	C(no)
	C(yes)
	CF(_zero,"0")
	CF(_one,"1")

	CF(_asterisk,"*")

	//control
	C(control)

	//Lighting
	C(lighting)
	C(default)
	C(last)

	CF(fade_rate,"fade-rate")


	C(command)
	C(activate)
	C(deactivate)
	C(goto)
	C(request)
	C(create)
	C(list)

	C(gateway)
	C(network)
//	C(device)
	C(channel)
	C(scene)
	C(level)
	C(pin)

	C(gateinfo)
	C(netlist)
	C(netinfo)
	C(devlist)
	C(scnlist)
	C(devinfo)
	C(scninfo)


	C(ok)
	CF(not_found,"not-found");

	CF(report_on_manual,"report-on-manual");
	CF(channel_count,"channel_count");
	CF(primary_channel,"primary-channel");

	CF(device_count,"device-count");
	CF(scene_count,"scene-count");
	CF(net_count,"net-count");
	CF(preferred_net,"preferred-net");
	CF(scenes_ok,"scenes-ok");
	CF(channels_ok,"channels-ok");
	CF(fade_rate_ok,"fade-rate-ok");

	C(protocol);
	CF(_protocol,"ARXPL");

	// HTML
	CF(html_header,
	"HTTP/1.0 200 OK\n"
    "Content-type: text/html\n"
    "Pragma: no-cache\n")
	C(h1)
	C(title)
	C(xPL)
	C(li)
	//C(dummytotestcompileroptimization)


	// LCD Debug

	CF(_blkline,"                    ");
