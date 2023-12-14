let mTxRx;
let connectButton;

function initTxRx() {
  // mTxRx = new SerialTxRx(9600);
  // mTxRx = new WiFiTxRx("http://192.168.1.100");
  mTxRx = new BLETxRx("e416af28-c3e9-43c2-a538-b2e27d9dcfb4");
  mTxRx.registerOnReceive((d) => print(d));
  connectButton.hide();
}

function setup() {
  createCanvas(windowWidth, windowHeight);

  connectButton = createButton("Connect To Arduino");
  connectButton.position(width / 2, height / 2);
  connectButton.mousePressed(initTxRx);
}

function draw() {
  background(0);

  if (mTxRx) {
    mTxRx.rx();
  }
}

function mousePressed() {
  if (mTxRx) {
    mTxRx.tx({ data: { x: mouseX, y: mouseY } });
  }
}
