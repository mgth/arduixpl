/*
			msgOut.addKey(S(device),id());
			msgOut.addKey(S(type),className());

			if (msg.key_request()==S(current)) msgAddCurrent(msgOut);
			if (msg.key_request()==S(name)) msgOut.addKey(S(name),id());
			xPL.sendMessage(msg);
			msg.addKey(S(device),id());
			msg.addKey(S(type),className());
			msgAddCurrent(msg);