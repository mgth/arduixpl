	C(xpl)
	C(stat)
	C(cmnd)
	C(trig)

	C(hop)
	C(source)
	C(target)

	C(test)

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

	//debug
	C(ArduixPL)
	C(parse)
	C(out)
	C(sent)
	C(send)
	C(received)
	C(mem)
	C(cost)

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
