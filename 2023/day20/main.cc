#include <aocutils.hh>
#include <bitset>
#include <iostream>
#include <memory>
#include <numeric>
#include <queue>
#include <unordered_map>

using AocType = unsigned long long;

struct Signal {
  std::string sender, receiver;
  // Pulse is bool - 0 for low and 1 for high
  bool pulse;

  Signal(const std::string &_sender, const std::string &_receiver, bool _pulse)
      : sender(_sender), receiver(_receiver), pulse(_pulse) {}
};

using SignalQueue = std::queue<std::unique_ptr<Signal>>;

class Module {
public:
  enum ModuleType { FlipFlop, Conjunction, Broadcaster };

  const std::string &getName() { return name; }

  ModuleType getType() { return type; }

  const std::vector<std::string> &getReceivers() { return receivers; }

  Module(const std::string &_line, SignalQueue &_sq, ModuleType _type)
      : sq(_sq), type(_type) {
    const std::vector<std::string> tokens = split(_line, "->, %&");
    auto token = tokens.begin();
    name = *token;
    token++;
    receivers.insert(receivers.begin(), token, tokens.end());
  }

  void addReceiver(const std::string &receiver) {
    receivers.push_back(receiver);
  }

  virtual void receiveSignal(const std::unique_ptr<Signal> &signal) = 0;

  void sendPulse(bool pulse) {
    for (const std::string &receiver : receivers) {
      /*std::cout << name << " -" << (pulse ? "high" : "low") << "-> " <<
         receiver
                << std::endl;*/
      sq.push(std::make_unique<Signal>(name, receiver, pulse));
    }
  }

  virtual void clear() {}

  virtual bool isFull() { return false; }

  virtual ~Module() {}

private:
  std::string name;
  std::vector<std::string> receivers;
  SignalQueue &sq;
  ModuleType type;
};

class FlipFlopModule : public Module {
  bool store{false};

public:
  FlipFlopModule(const std::string &_line, SignalQueue &_sq)
      : Module(_line, _sq, ModuleType::FlipFlop) {}

  void receiveSignal(const std::unique_ptr<Signal> &signal) {
    if (signal->pulse) {
      // high pulse - do nothing
      return;
    }

    // flip and send signal
    store = !store;
    sendPulse(store);
  }

  void clear() { store = false; }
};

class ConjunctionModule : public Module {
  std::unordered_map<std::string, bool> pulseStore;

public:
  ConjunctionModule(const std::string &_line, SignalQueue &_sq)
      : Module(_line, _sq, ModuleType::Conjunction) {}

  void addSender(const std::string &sender) { pulseStore[sender] = false; }

  void receiveSignal(const std::unique_ptr<Signal> &signal) {
    pulseStore[signal->sender] = signal->pulse;

    for (const auto &entry : pulseStore) {
      if (!entry.second) {
        sendPulse(true);
        return;
      }
    }

    // all high signals
    sendPulse(false);
  }

  virtual bool isFull() {
    for (const auto &entry : pulseStore) {
      if (!entry.second) {
        return false;
      }
    }

    return true;
  }

  void clear() {
    for (auto entry : pulseStore) {
      entry.second = false;
    }
  }
};

class BroadcasterModule : public Module {
public:
  BroadcasterModule(const std::string &_line, SignalQueue &_sq)
      : Module(_line, _sq, ModuleType::Broadcaster) {}

  void receiveSignal(const std::unique_ptr<Signal> &signal) {
    // forward it to connected modules
    sendPulse(signal->pulse);
  }
};

class Processor {
  SignalQueue sigQueue;
  std::unordered_map<std::string, std::unique_ptr<Module>> modules;
  bool setupComplete{false};

public:
  void addModule(const std::string &line) {
    std::unique_ptr<Module> module;
    switch (line[0]) {
    case '%':
      module = std::make_unique<FlipFlopModule>(line, sigQueue);
      break;
    case '&':
      module = std::make_unique<ConjunctionModule>(line, sigQueue);
      break;
    case 'b':
      module = std::make_unique<BroadcasterModule>(line, sigQueue);
      break;

    default:
      break;
    }
    std::string name = module->getName();
    modules[name] = std::move(module);
    setupComplete = false;
  }

  void completeSetup() {
    if (setupComplete) {
      return;
    }

    // update senders in all Conjunction modules
    for (const auto &entry : modules) {
      for (const auto &moduleName : entry.second->getReceivers()) {
        auto it = modules.find(moduleName);
        if (it == modules.end()) {
          continue;
        }

        std::unique_ptr<Module> &m = it->second;
        if (m->getType() == Module::ModuleType::Conjunction) {
          (dynamic_cast<ConjunctionModule *>(m.get()))->addSender(entry.first);
        }
      }
    }

    setupComplete = true;
  }

  void resetModules() {
    for (const auto &entry : modules) {
      entry.second->clear();
    }
  }

  std::pair<AocType, AocType> pushButtonPart1() {
    if (!setupComplete) {
      return {0, 0};
    }

    sigQueue.push(std::make_unique<Signal>("processor", "broadcaster", false));

    AocType lowPulses = 0, highPulses = 0;

    // handle all the signals
    std::unique_ptr<Signal> signal;
    while (!sigQueue.empty()) {
      signal = std::move(sigQueue.front());
      sigQueue.pop();

      if (signal->pulse) {
        highPulses++;
      } else {
        lowPulses++;
      }

      auto it = modules.find(signal->receiver);
      if (it == modules.end()) {
        continue;
      }

      std::unique_ptr<Module> &m = it->second;
      m->receiveSignal(signal);
    }

    return {lowPulses, highPulses};
  }

  void pushButtonPart2(std::vector<std::string> &senders,
                       std::bitset<4> &cycleFound, std::vector<int> &cycle,
                       int loopNum) {
    if (!setupComplete) {
      return;
    }

    sigQueue.push(std::make_unique<Signal>("processor", "broadcaster", false));

    // handle all the signals
    std::unique_ptr<Signal> signal;
    while (!sigQueue.empty()) {
      signal = std::move(sigQueue.front());
      sigQueue.pop();

      auto sendersIt =
          std::find(senders.begin(), senders.end(), signal->sender);

      if (sendersIt != senders.end() && signal->pulse) {
        auto idx = std::distance(senders.begin(), sendersIt);
        if (!cycleFound.test(idx)) {
          cycleFound.set(idx);
          cycle.push_back(loopNum);
        }
      }

      auto it = modules.find(signal->receiver);
      if (it == modules.end()) {
        continue;
      }

      std::unique_ptr<Module> &m = it->second;
      m->receiveSignal(signal);
    }
  }

  std::vector<std::string> findConnectedSenders(const std::string &curr) {
    std::vector<std::string> senders;
    for (const auto &entry : modules) {
      for (const auto &moduleName : entry.second->getReceivers()) {
        if (moduleName == curr) {
          senders.push_back(entry.first);
        }
      }
    }

    return std::move(senders);
  }

  AocType solveForRX() {
    std::vector<int> cycle;
    std::bitset<4> cycleFound;

    // Answer is probably too big of a value to run in loop, so find a cycle
    // among its sender/sender's of senders and calculate LCM rx's sender
    // is a &module and it inturn has 4 &module senders.
    std::vector<std::string> senders =
        findConnectedSenders(findConnectedSenders("rx")[0]);

    resetModules();

    int i = 0;
    do {
      i++;
      pushButtonPart2(senders, cycleFound, cycle, i);
    } while (!cycleFound.all());

    AocType lcmValue = 1;
    for (AocType c : cycle) {
      lcmValue = std::lcm<AocType, AocType>(lcmValue, c);
    }

    return lcmValue;
  }
};

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  InputReader input(20, &args);

  Processor processor;

  for (auto it = input.begin(); it != input.end(); ++it) {
    processor.addModule(*it);
  }
  processor.completeSetup();

  std::pair<AocType, AocType> pulses = {0, 0};
  for (int i = 0; i < 1000; i++) {
    auto res = processor.pushButtonPart1();
    pulses.first += res.first;
    pulses.second += res.second;
  }

  std::cout << "Total number of pulses (Part I) : "
            << pulses.first * pulses.second << std::endl;

  // Part II
  std::cout << "Required number of button pushes to turn on rx (Part II) : "
            << processor.solveForRX() << std::endl;
}
