//--------------------------INICIALIZACION DE ELEMENTOS--------------------------
const jugarJuego = document.getElementById('jugarJuego'); 
const reglas = document.querySelector('.reglas');
const character = document.querySelector('.character');
const sword = document.querySelector('.sword');
const enemy = document.querySelector('.enemy');
const meteor = document.querySelector('.enemyAttack');
const esquivar = document.querySelector('.esquivar');
const atacar = document.querySelector('.atacar');
var lecturaDanio=document.getElementById("danio");
var lecturaVidaEne=document.getElementById("vidaEne");
var lecturaRondas=document.getElementById("rondas");

//--------------------------VARIABLES PARA GUARDAR EL ESTADO DE LOS BOTONES--------------------------
var stateAtq = 1; // Inicializa con un valor 1 que significa que el boton de ataque no ha sido presionado 
var stateEsq = 0; // Inicializa con un valor 0 que significa que el boton de esquivar no ha sido presionado 

//--------------------------FUNCIONES PARA MOSTRAR LOS VALORES EN TIEMPO REAL EN HTML--------------------------
function leerRondas(valor){
    lecturaRondas.innerHTML="RONDA: "+ valor;
}
function leerVidaEne(valor){
    lecturaVidaEne.innerHTML=valor+" VIDA";
}
function leerDanio(valor){
    lecturaDanio.innerHTML=valor+" DAÑO";
}

//--------------------------DEFINIR ANIMACIONES DEL JUGADOR Y LA HIDRA--------------------------
function jugadorAtacaEnemigoEsquiva() {
    enemy.classList.add('animateEnemyDodge');
    character.classList.add('animateCharacter');
    sword.classList.add('animateSword');
    atacar.classList.add('botonPresionado2');

    // Elimina las clases de animación después de que termine la animación
    setTimeout(() => {
        enemy.classList.remove('animateEnemyDodge');
        character.classList.remove('animateCharacter');
        sword.classList.remove('animateSword');
        atacar.classList.remove('botonPresionado2');
    }, 1500);  
}

function enemigoAtacajugadorEsquiva() {
    meteor.classList.add('animateEnemyAttack');
    character.classList.add('animateCharacterDodge');
    sword.classList.add('animateSwordDodge');
    esquivar.classList.add('botonPresionado1');

    // Elimina las clases de animación después de que termine la animación
    setTimeout(() => {
        meteor.classList.remove('animateEnemyAttack');
        character.classList.remove('animateCharacterDodge');
        sword.classList.remove('animateSwordDodge');
        esquivar.classList.remove('botonPresionado1');
    }, 1500);  
}

function ambosAtacan(){
    meteor.classList.add('animateEnemyAttack');
    character.classList.add('animateCharacter');
    sword.classList.add('animateSword');
    atacar.classList.add('botonPresionado2');

    // Elimina las clases de animación después de que termine la animación
    setTimeout(() => {
        meteor.classList.remove('animateEnemyAttack');
        character.classList.remove('animateCharacter');
        sword.classList.remove('animateSword');
        atacar.classList.remove('botonPresionado2');
    }, 1500);  
}

function ambosEsquivan(){
    enemy.classList.add('animateEnemyDodge');
    character.classList.add('animateCharacterDodge');
    sword.classList.add('animateSwordDodge');
    esquivar.classList.add('botonPresionado1');

    // Elimina las clases de animación después de que termine la animación
    setTimeout(() => {
        enemy.classList.remove('animateEnemyDodge');
        character.classList.remove('animateCharacterDodge');
        sword.classList.remove('animateSwordDodge');
        esquivar.classList.remove('botonPresionado1');
    }, 1500);  
}

function jugadorGana() {
    enemy.classList.add('animateDisappear');
    character.classList.add('animateLeftC');
    sword.classList.add('animateLeftS');
}

function enemigoGana() {
    enemy.classList.add('animateLeftE');
    character.classList.add('animateDC');
    sword.classList.add('animateDS');
}

function empate() {
    enemy.classList.add('aEmpateE');
    character.classList.add('aEmpateC');
    sword.classList.add('aEmpateS');
}


function mostrarAnimacionDanio() {
    // Hace visible el elemento y agrega la clase fadeIn
    lecturaDanio.style.visibility = 'visible';
    lecturaDanio.classList.add('fadeIn');

    // Luego de un tiempo, oculta nuevamente el elemento y elimina la clase fadeIn
    setTimeout(() => {
        lecturaDanio.style.visibility = 'hidden';
        lecturaDanio.classList.remove('fadeIn');
    }, 900);  // Duración de la animación en milisegundos
}

function mostrarAnimacionVidaEne() {
    // Hace visible el elemento y agrega la clase fadeIn
    lecturaVidaEne.style.visibility = 'visible';
    lecturaVidaEne.classList.add('fadeIn');

    // Luego de un tiempo, oculta nuevamente el elemento y elimina la clase fadeIn
    setTimeout(() => {
        lecturaVidaEne.style.visibility = 'hidden';
        lecturaVidaEne.classList.remove('fadeIn');
    }, 900);  // Duración de la animación en milisegundos
}

function ocultarRondas() {
    lecturaRondas.classList.add('oculto');
}

//--------------------------FUNCIONES PARA LEER LOS VALORES DE ARDUINO--------------------------
setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            // Convierte la respuesta a un entero
            var lightValue = parseInt(this.responseText, 10);
            
            // Llama a la función para cambiar el fondo con el valor obtenido
            changeBackground(lightValue);
        }
    };
    xhttp.open("GET", "/sensor", true);
    xhttp.send();
}, 100);

setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            // Convierte la respuesta a un entero
            stateAtq = parseInt(this.responseText, 10);
        }
    };
    xhttp.open("GET", "/ataque", true);
    xhttp.send();
}, 100);

setInterval(function () {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            // Convierte la respuesta a un entero
            stateEsq = parseInt(this.responseText, 10);
        }
    };
    xhttp.open("GET", "/esquivar", true);
    xhttp.send();
}, 100);

async function enviarDatosArduino(vida) {
    const xhttp = new XMLHttpRequest();
    xhttp.open("GET", `/controlar_leds?vida=${vida}`, true);
    xhttp.send();
} 
//--------------------------FUNCIONES PARA LA LOGICA INTERNA DEL JUEGO --------------------------
function changeBackground(lightValue) {
    const background = document.getElementById('background');
    const basePath = window.location.origin;

    if (lightValue >= 0 && lightValue <= 1364) {
        
        background.style.backgroundImage = `url(${basePath}/Noche.png)`;
    } else if (lightValue >= 1365 && lightValue <= 2730) {
        
        background.style.backgroundImage = `url(${basePath}/Tarde.png)`;
    } else if (lightValue >= 2731 && lightValue <= 4095) {
        
        background.style.backgroundImage = `url(${basePath}/Dia.png)`;
    } else {
        console.log('Valor no válido');
    }
}


function enemigo(){
    let generarAtaqueEnemigo=false;
    var numeroAleatorio = Math.random();
    numeroAleatorio *= 2;
    var resultado = Math.floor(numeroAleatorio);
        if(resultado==1){
            generarAtaqueEnemigo = true;
            console.log("El enemigo ha atacado");
        }        
        if(resultado==0){
            generarAtaqueEnemigo = false;
            console.log("El enemigo ha esquivado");
        }
    return generarAtaqueEnemigo;   
    }  

//Funcion principal que maneja la logica del juego, se activa por medio de un click al boton "INICIAR COMBATE"
jugarJuego.addEventListener('click', async function () {
    reglas.classList.add('animateReglasDisappear');
    this.classList.add('botonDesaparecido');

    //Parte del codigo que envia el permiso a Arduino para activar la musica
    const xhttpStartGame = new XMLHttpRequest();
    xhttpStartGame.open("GET", "/start_game", true);
    xhttpStartGame.send();

    let eContador = 0;
    let jContador = 0;
    let eDanioAtq = 1;
    let jDanioAtq = 1;
    let eVida = 5;
    let jVida = 5;
    let jugadorAtq = false;
    let jugadorEsq = false;
    let leerAtaqueEnemigo = false;
    let rondas = 0;
    console.log("COMBATE INICIADO");

    async function waitForPlayerChoice() {
        return new Promise(resolve => {
            console.log("Es tu turno de atacar ");
            const intervalId = setInterval(() => {
                if (stateAtq === 0 || stateEsq === 1) {
                    clearInterval(intervalId);
                    enviarDatosArduino(jVida);
                    resolve();
                }
            }, 100);
        });
    }

    while (eVida > 0 && jVida > 0) {
        rondas++;
        leerRondas(rondas);
        console.log("RONDA " + rondas);

        await waitForPlayerChoice();

        jugadorAtq = stateAtq == 0;
        jugadorEsq = stateEsq == 1;

        leerAtaqueEnemigo = enemigo();

    if(jugadorAtq & leerAtaqueEnemigo){
        ambosAtacan();
        jVida = jVida - eDanioAtq;
        eVida = eVida - jDanioAtq;
        eContador = 0;
        jContador = 0;
        eDanioAtq=1;
        jDanioAtq = 1;
        stateAtq = 1;
        stateEsq = 0;
        leerVidaEne(eVida);
        leerDanio(jDanioAtq);
        await enviarDatosArduino(jVida);
        mostrarAnimacionVidaEne();
        mostrarAnimacionDanio();
        console.log("-----------------CASO 1-----------------" );
        console.log("Vida actual del jugador " + jVida);
        console.log("Vida actual del enemigo " + eVida);
        console.log("Daño actual del jugador " + jDanioAtq);
        console.log("Daño actual del enemigo " + eDanioAtq);
        console.log("Contador actual del jugador " + jContador);
        console.log("Contador actual del enemigo " + eContador);
    }else if(jugadorAtq & !leerAtaqueEnemigo){
        jugadorAtacaEnemigoEsquiva();
        eDanioAtq++;
        jDanioAtq = 1;
        eContador++;
        jContador = 0;
        stateAtq = 1;
        stateEsq = 0;
        leerDanio(jDanioAtq);
        mostrarAnimacionDanio();
        console.log("-----------------CASO 2-----------------" );
        console.log("Vida actual del jugador " + jVida);
        console.log("Vida actual del enemigo " + eVida);
        console.log("Daño actual del jugador " + jDanioAtq);
        console.log("Daño actual del enemigo " + eDanioAtq);
        console.log("Contador actual del jugador " + jContador);
        console.log("Contador actual del enemigo " + eContador);
    }else if(jugadorEsq & !leerAtaqueEnemigo){
        ambosEsquivan();
        eContador++;
        jContador++;
        stateAtq = 1;
        stateEsq = 0;
        leerDanio(jDanioAtq);
        mostrarAnimacionDanio();
        console.log("-----------------CASO 3-----------------" );
        console.log("Vida actual del jugador " + jVida);
        console.log("Vida actual del enemigo " + eVida);
        console.log("Daño actual del jugador " + jDanioAtq);
        console.log("Daño actual del enemigo " + eDanioAtq);
        console.log("Contador actual del jugador " + jContador);
        console.log("Contador actual del enemigo " + eContador);
    }else if(jugadorEsq & leerAtaqueEnemigo){
        enemigoAtacajugadorEsquiva();
        jDanioAtq++;
        eDanioAtq = 1;
        eContador=0;
        jContador++;
        stateAtq = 1;
        stateEsq = 0;
        leerDanio(jDanioAtq);
        mostrarAnimacionDanio();
        console.log("-----------------CASO 4-----------------" );
        console.log("Vida actual del jugador " + jVida);
        console.log("Vida actual del enemigo " + eVida);
        console.log("Daño actual del jugador " + jDanioAtq);
        console.log("Daño actual del enemigo " + eDanioAtq);
        console.log("Contador actual del jugador " + jContador);
        console.log("Contador actual del enemigo " + eContador);
    }
    
    if(jContador==3){
        jDanioAtq=1;
        jContador=0;
        console.log("Daño actual del jugador " + jDanioAtq);
        console.log("Contador actual del jugador " + jContador);
    }
    if(eContador==3){
        eDanioAtq=1;
        eContador=0;
        console.log("Daño actual del enemigo " + eDanioAtq);
        console.log("Contador actual del enemigo " + eContador);
    }
    const delayTime = 800; 
    await delay(delayTime);
    } 
    ocultarRondas();
    if(eVida>0 & jVida<=0){
        console.log("LA HIDRA HA GANADO");
        document.querySelector('.letreroPerder').classList.add('mostrar-letrero');
        enemigoGana();
    }else if(eVida<=0 & jVida>0){
        console.log("FELICIDADES HAS GANADO");
        document.querySelector('.letreroGanar').classList.add('mostrar-letrero');
        jugadorGana();
    } else{
        console.log("EL COMBATE HA TERMINADO EN EMPATE");
        document.querySelector('.letreroEmpate').classList.add('mostrar-letrero');
        empate();
    }
});

const delay = async (ms) => new Promise(resolve => setTimeout(resolve, ms));