const char HTML_CONTENT[] PROGMEM = R"KEWL(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="shortcut icon" href="https://neondra.com/neondra/assets/images/logo/favicon.png" type="image/x-icon">
    <title>Neondra Interface</title>

    <style>
        body {
            background-color: gray;
        }
        main {
            display: inline-flex;
            flex-direction: column;
        }
        button {
            margin: 1px;
        }
    </style>
</head>
<body>
    <main>
        <button onclick="setBrightness('0');">Set Brightness 0%</button>
        <button onclick="setBrightness('20');">Set Brightness 20%</button>
        <button onclick="setBrightness('40');">Set Brightness 40%</button>
        <button onclick="setBrightness('60');">Set Brightness 60%</button>
        <button onclick="setBrightness('80');">Set Brightness 80%</button>
        <button onclick="setBrightness('100');">Set Brightness 100%</button>
    
        <button onclick="playPrevious();">Play Previous</button>
        <button onclick="togglePlay();">Play / Pause</button>
        <button onclick="playNext();">Play Next</button>
        <button onclick="mVolumeUp();">M Volume Up</button>
        <button onclick="mVolumeDown();">M Volume Down</button>
        
        <button onclick="playAudioSfx('1');">SFX 1</button>
        <button onclick="playAudioSfx('2');">SFX 2</button>
        <button onclick="playAudioSfx('3');">SFX 3</button>
        <button onclick="playAudioSfx('4');">SFX 4</button>
        <button onclick="playAudioSfx('5');">SFX 5</button>
        <button onclick="playAudioSfx('6');">SFX 6</button>
        <button onclick="playAudioSfx('7');">SFX 7</button>
        <button onclick="playAudioSfx('8');">SFX 8</button>
        
        <button onclick="setFanSpeed('0');">Set Fan 0%</button>
        <button onclick="setFanSpeed('20');">Set Fan 20%</button>
        <button onclick="setFanSpeed('40');">Set Fan 40%</button>
        <button onclick="setFanSpeed('60');">Set Fan 60%</button>
        <button onclick="setFanSpeed('80');">Set Fan 80%</button>
        <button onclick="setFanSpeed('100');">Set Fan 100%</button>
        
        <button onclick="playAnim('boop');">Play Anim Boop</button>
        <button onclick="playAnim('wink');">Play Anim Wink</button>
        <button onclick="playAnim('happy');">Play Anim Happy</button>
        <button onclick="playAnim('cute');">Play Anim Cute</button>
        <button onclick="playAnim('swag');">Play Anim Swag</button>
        <button onclick="playAnim('rave');">Play Anim Rave</button>
        <button onclick="playAnim('angry');">Play Anim Angry</button>

        <button onclick="setWingPosition('0');">Set Wing Position 0</button>
        <button onclick="setWingPosition('20');">Set Wing Position 20</button>
        <button onclick="setWingPosition('40');">Set Wing Position 40</button>
        <button onclick="setWingPosition('60');">Set Wing Position 60</button>
        <button onclick="setWingPosition('80');">Set Wing Position 80</button>
        <button onclick="setWingPosition('100');">Set Wing Position 100</button>
    </main>
    
    <script type="text/javascript">
        // AJAX Requests
        function sendCommand(data) {
            fetch('/ajax', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'plain=' + encodeURIComponent(data),
            }).then(response => response.text()).then(result => {
                console.log('Response:', result);
            }).catch(error => {
                console.error('Error:', error);
            });
        }
        function togglePlay() {
            sendCommand(`play='toggle'`);
        }
        function playNext() {
            sendCommand(`play='next'`);
        }
        function playPrevious() {
            sendCommand(`play='previous'`);
        }
        function playAudioM(name) {
            sendCommand(`play-audio-m=${name}`);
        }
        function playAudioSfx(name) {
            sendCommand(`play-audio-sfx=${name}`);
        }
        function mVolumeUp() {
            sendCommand(`set-volume-m='up'`);
        }
        function mVolumeDown() {
            sendCommand(`set-volume-m='down'`);
        }
        function playAnim(name) {
            sendCommand(`play-anim=${name}`);
        }
        function setFanSpeed(percent) {
            sendCommand(`set-fan=${percent}`);
        }
        function setBrightness(percent) {
            sendCommand(`set-brightness=${percent}`);
        }
        function setWingPosition(percent) {
            sendCommand(`set-wing-position=${percent}`);
        }
    </script>
</body>
</html>
)KEWL";