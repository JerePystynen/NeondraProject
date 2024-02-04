const char HTML_CONTENT[] PROGMEM = R"KEWL(
<html>
<head>
  <link rel="icon" type="image/x-icon" href="https://neondra.com/neondra/assets/images/logo/favicon.png">
  <title>Neondra Control Panel</title>
  <style>
    * {
      margin:0;
      padding:0;
    }
    body {
      display: flex;
      flex-direction: column;
      background-color: #2E2532;
      color: #ECFBFF;
      font-size: 18px;
      min-height: 100vh;
      width: 100vw;
      font-family: sans-serif;
    }
    a {
      color: #631D76;
      margin: 6px 0;
      text-underline: none;
    }
    a:visited {
      color: #631D76;
    }
    img {
      width: 32px;
    }
  </style>
</head>
<body>
  <h1>Welcome to Neondra's control panel, powered by ESP8266!</h1>

  <span>Now displaying: -</span>
  <span>Now playing: -</span>
  
  <h2>Set What Helmet Matrices Display:</h2>
  <a href="/?set-visor=default/">DEFAULT</a>
  <a href="/?set-visor=wink/">WINK</a>
  <a href="/?set-visor=love/">LOVE</a>
  <a href="/?set-visor=sad/">SAD</a>
  <a href="/?set-visor=fury/">FURY</a>
  <a href="/?set-visor=vumeter">Display Music Visualizer</a>
  <a href="/?set-visor=datetime">Display Datetime</a>
  <a href="/?set-visor=subcount">Display Subcount</a>
  <a href="/?set-animation-playstate=true">Play Animation</a>
  <a href="/?set-animation-playstate=false">Stop Animation</a>
  <h2>Set Matrix Color:</h2></br>
  <a href="/?set-visor=0000FF">Set Blue</a>
  <a href="/?set-visor=00FF00">Set Green</a>
  <a href="/?set-visor=FF0000">Set Red</a>
  <h2>Play Audio:</h2>
  <a href="/?play-audio=1">Play Audio 1</a>
  <a href="/?play-audio=2">Play Audio 2</a>
  <a href="/?play-audio=3">Play Audio 3</a>
  <a href="/?play-audio=0">Stop Playing</a>
</body>
</html>
)KEWL";
