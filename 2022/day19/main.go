package main

import "fmt"

type robotFactory struct {
	// blueprint information
	bid, ore_ore, clay_ore, obs_ore, obs_clay, geo_ore, geo_obs int
	// inventory
	ore, clay, obs, geode int
	// robots
	oreRobot, clayRobot, obsRobot, geodeRobot int
}

func (f *robotFactory) qualityLevel(minutes int) int {

	for minutes > 0 {
		// Build robots
		// geode
		var oreRobotWIP, clayRobotWIP, obsRobotWIP, geodeRobotWIP int
		for f.ore >= f.geo_ore && f.obs >= f.geo_obs {
			f.ore -= f.geo_ore
			f.obs -= f.geo_obs
			geodeRobotWIP++
		}
		// obs
		for f.ore >= f.obs_ore && f.clay >= f.obs_clay {
			f.ore -= f.obs_ore
			f.clay -= f.obs_clay
			obsRobotWIP++
		}
		// clay
		for f.ore >= f.clay_ore {
			f.ore -= f.clay_ore
			clayRobotWIP++
		}
		// ore
		for f.ore >= f.ore_ore {
			f.ore -= f.ore_ore
			oreRobotWIP++
		}

		// collect
		f.ore += f.oreRobot
		f.clay += f.clayRobot
		f.obs += f.obsRobot
		f.geode += f.geodeRobot

		// finish robot construction
		f.oreRobot += oreRobotWIP
		f.clayRobot += clayRobotWIP
		f.obsRobot += obsRobotWIP
		f.geodeRobot += geodeRobotWIP

		fmt.Println(f)

		minutes--
	}

	return f.geode * minutes
}

func main() {
	for {
		var factory robotFactory
		if n, _ := fmt.Scanf("Blueprint %d: "+
			"Each ore robot costs %d ore. "+
			"Each clay robot costs %d ore. "+
			"Each obsidian robot costs %d ore and %d clay. "+
			"Each geode robot costs %d ore and %d obsidian.\n",
			&factory.bid, &factory.ore_ore, &factory.clay_ore, &factory.obs_ore, &factory.obs_clay, &factory.geo_ore, &factory.geo_obs); n == 0 {
			break
		}

		factory.oreRobot = 1

		fmt.Println(factory.qualityLevel(24))
	}
}
