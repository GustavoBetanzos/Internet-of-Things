function updateSliderPWM(element) {
  var sliderValue = document.getElementById("pwmSlider").value;
  document.getElementById("textSliderValue").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value=" + sliderValue, true);
  xhr.send();
}

function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if (element.checked) {
      xhr.open("GET", "/update?output=" + element.id + "&state=1", true);
  } else {
      xhr.open("GET", "/update?output=" + element.id + "&state=0", true);
  }
  xhr.send();

  // Reemplazar el marcador de posición con los nuevos botones
  var buttonPlaceholder = document.getElementById("BUTTONPLACEHOLDER");
  buttonPlaceholder.innerHTML = ""; // Limpia el marcador de posición
}


// Ejecutar la función que obtiene el valor del sensor LDR cada 100ms
setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
      if (this.readyState == 4 && this.status == 200) {
          document.getElementById("SENSOR_ID").innerHTML = this.responseText;
      }
  };
  xhttp.open("GET", "/sensor", true);
  xhttp.send();
}, 100);

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("POT_ID").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/pot", true);
  xhttp.send();
}, 2000 ) ;

