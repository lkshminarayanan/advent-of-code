#include <aocutils.hh>
#include <iostream>
#include <memory>

using AocType = long;
AocType calculateHash(const std::string &step) {
  AocType currentValue = 0;
  for (char c : step) {
    currentValue += int(c);
    currentValue *= 17;
    currentValue %= 256;
  }
  return currentValue;
}

struct Lens {
  std::string _label;
  int _focalLength;

  std::shared_ptr<Lens> prev{nullptr};
  std::shared_ptr<Lens> next{nullptr};

  Lens(std::string label, int focalLength)
      : _label(label), _focalLength(focalLength) {}
};

class Box {
  // front has a dummy head
  std::shared_ptr<Lens> front = std::make_shared<Lens>("head", -1);
  std::shared_ptr<Lens> back = front;

  std::shared_ptr<Lens> find(const std::string &label) const {
    std::shared_ptr<Lens> temp = front;
    while (temp != nullptr && temp->_label != label) {
      temp = temp->next;
    }

    return temp;
  }

public:
  bool isEmpty() { return front == back; }

  void addLens(const std::string &label, int focalLength) {
    auto lens = find(label);
    if (lens) {
      // lens exists - update it
      lens->_focalLength = focalLength;
    } else {
      // lens doesn't exist.
      // add new lens to the back
      auto lens = std::make_shared<Lens>(label, focalLength);
      back->next = lens;
      lens->prev = back;
      back = lens;
    }
  }

  void removeLens(const std::string &label) {
    auto lens = find(label);
    if (lens) {
      // lens exists - delete it
      if (back == lens) {
        // lens is last
        back = lens->prev;
        back->next = nullptr;
      } else {
        lens->prev->next = lens->next;
        lens->next->prev = lens->prev;
      }
    }
  }

  AocType calculateFocusPower() {
    AocType power = 0;
    int lensSlot = 1;
    std::shared_ptr<Lens> temp = front->next;
    while (temp != nullptr) {
      power += lensSlot++ * temp->_focalLength;
      temp = temp->next;
    }

    return power;
  }

  std::string toString() {
    std::string output;
    std::shared_ptr<Lens> temp = front->next;
    while (temp != nullptr) {
      output += " -> [" + temp->_label + " : " +
                std::to_string(temp->_focalLength) + "]";
      temp = temp->next;
    }

    return output;
  }
};

class BoxCollection {
  Box boxes[256];

public:
  void processStep(const std::string &step) {
    auto it = step.find_first_of("=-");
    const std::string label = step.substr(0, it);
    const int labelHash = calculateHash(label);

    switch (step[it]) {
    case '=':
      boxes[labelHash].addLens(label, step[it + 1] - '0');
      break;

    case '-':
      boxes[labelHash].removeLens(label);
      break;
    }
  }

  AocType calculateFocusPower() {
    AocType power = 0;
    for (int i = 0; i < 256; i++) {
      if (!boxes[i].isEmpty()) {
        power += boxes[i].calculateFocusPower() * (i + 1);
      }
    }

    return power;
  }

  void print() {
    for (int i = 0; i < 256; i++) {
      if (!boxes[i].isEmpty()) {
        std::cout << i << boxes[i].toString() << std::endl;
      }
    }
  }
};

int main(int argc, char *argv[]) {
  AocCmdLineArgs args(argc, argv);
  std::string initialisationSequence = InputReader(15, &args).readAll();
  auto steps = split(initialisationSequence, ",\n");

  AocType sumOfAllHash = 0;
  for (const std::string &step : steps) {
    sumOfAllHash += calculateHash(step);
  }

  std::cout << "Sum of all hash (Part I) : " << sumOfAllHash << std::endl;

  // Part II
  BoxCollection bc;
  for (const std::string &step : steps) {
    bc.processStep(step);
  }

  std::cout << "Total focus power (Part II) : " << bc.calculateFocusPower()
            << std::endl;
}
