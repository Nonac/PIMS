### IoT
<table id = "2bIoTTb">

  <tr>
    <th>Technique</th>
    <th>pros / Resons of choice</th>
    <th>cons / Limitations</th>
  </tr>
  
  <tr>
    <td>Making M5Stack the controller of a barrier</td>
    <td><ul>
    <li> M5Stacks are great, versatile prototyping tools with various built-in functionalities such as BlueTooth, WiFi, and analog/digital input/output </li>
    <li> The processor of an M5Stack is a 240 MHz dual-core Tensilica LX6 microcontroller, which is fast enough to perform our tasks. </li>
    <li> An M5Stack has a 320x240 Colorful TFT LCD and 3 programmable buttons, which makes it ideal for simulating a real barrier.</li>
    </ul></td>
    <td><ul>
    <li><a href = "/Report/System_implementation/README.md/#2aIoT_m5memory">The memory for storing programs on an M5Stack is limited, which means a large program may have to be split into parts run by multiple devices that communicate with each other.</a></li>
    <li>The compilation time for an M5Stack is much longer than a normal Arduino board<del>, which is very unfriendly to newbie programmers like me who will never run out of bugs to fix.</del></li>
    </ul></td>
  </tr>
  
  <tr>
    <td>Using M5Stick-Cs as keys</td>
    <td><ul>
    <li><a href = "/Report/System_implementation/README.md/#2aIoT_keysPowerLevel">Determining distance via RSSI is only valid when the BT modules of all the keys are on the same level. </a></li>
    <li>Each M5Stick-C has a display and a button just resembling hardware bank tokens.</li>
    <li>Its size is small enough to be a key.</li>
    </ul></td>
    <td><ul>
    <li>Its battery may not sustain its power consumption for a very long time.</li>
    <li>Its manufacturing cost may be too high for mass production.</li>
    </ul></td>
  </tr>
  
  <tr>
  <td>Using BT address as identifications</td>
  <td><ul>
  <li>BT addresses are designed to be unique and are a bit harder to fabricate compared to other types of ids such as MAC addresses.</li>
  <li>Generally, BlueTooth consumes less energy than WiFi. For IoT devices, Bluetooth Low Energy (BLE) could be utilised to save more energy.</li>
  <li>BlueTooth is designed to facilitate data transfer between devices over short distances, which suits our situation pretty well: short-range communication between keys and barriers.</li>
  </ul></td>
  <td>
  <li>There are existing technologies that consume even less energy such as Radio-frequency identification (RFID). If we do not add more security measures such as making the barriers paring the keys and ask for more authentication keys thereafter, our BLE approach may not outperform RFID except that it is easier for the developer to implement<del>, which will never be the concern for project managers.</del></li>
  </td>
  </tr>
</table>
