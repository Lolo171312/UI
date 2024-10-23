HUD:
- Barra de vida del jugador:
	Se conecta a un Delegate del Player en su función Show y se desconecta de dicho Delegate en su función Hide.
	Este Delegate sucede cada vez que el Jugador recibe daño y envía 2 ints (valor de VidaActual y el valor de VidaMáxima)
	La función ejecutada en PlayerHealthBar por el Delegate del Player modifica el Percentege de la Barra de vida. Además, si el percentege es < BLINK_THRESHOLD (0.25f) hará que una variable llamada bIsLowHealth sea = true;
	Cuando esa variable es "true" una función llamada LowHealthBlink en el NativeTick de PlayerHealthBar comienza a ejecutarse.
	Esta función tiene un timer que va continuamente creciente y decreciente (de 0 a 1 y vuelta de 1 a 0). Con ese valor se hace un Lerp entre el color base del Background de la barra y el nuevo color que esta tiene cuando la vida es baja (en este caso está puesto como rojo, pero se puede cambiar desde inspector)
	
- Crosshair dinámico:
	El script Crosshair se comunica con el WeaponComponent mediante 2 Delegates los cuales son OnWeaponFire y OnAimToEnemy. Ambos Delegates hacen Bind en la función Show de Crosshair y Unbind en la función Hide de Crosshair.
	OnWeaponFire se ejecuta cuando hace Spawn de una bala y no tiene ningún parámetro de entrada.
	OnAimToEnemy se va ejecutando contínuamente en el Tick de WeaponComponent y envía un bool como parámetro. Dicho bool será true si el arma está apuntando a un AUTAD_UI_FPS_Enemy o false si apunta a cualquier otra cosa.
	
	OnWeaponFire ejecuta la función Shoot de Crosshair. Esta función hace PlayAnimation de una animación de Widget. Dicha animación la hice desde inspector y obtengo su referencia en C++ con las UPROPERTY(Transcient, meta=("BindWidgetAnim")).
	En lugar de esta animación podría haber hecho un movimiento de la Img del crosshair usando el Tick, sin embargo, ya que Unreal permite animar elementos de los Widgets vi bien usar esta opción, además da lugar a un código más limpio.
	
	OnAimToEnemy ejecuta la función SetIsAimingToEnemy en el Crosshair. Dicha función llama a una función propia de la clase de Unreal llamada UImage que se llama SetColorAndOpacity. Si el valor introducido a la función es true el nuevo color será uno que puede ser modificable desde inspector (en este caso es rojo), y en caso de ser false el nuevo color será el original de la imagen.

- Marcador de daño en pantalla
	La forma en la que he hecho el indicador de daño consiste en un círculo dividido en cuatro partes para indicar al jugador la dirección del atacante. Cuando el jugador recibe daño un fragmento del círuculo modifica su opacidad a 1, se mantiene en 1 durante unos segundos y pasados dichos segundos comienza a modificar su opacidad a 0 de forma progresiva.
	Para comenzar, el script de DamageIndicator se conecta al Delegate OnReceiveDamage del jugador en la función Show y se desconecta en su función Hide. Este Delegate sucede cuando el jugador recibe daño y obtiene como argumentos de entrada un puntero al jugador y una referencia a un FVector que representa la posición del atacante en el momento en el que se recibió el daño.
	Cuando el Delegate es ejecutado se llama a la función StartDamageIndicator del DamageIndicator. Esta función crea un nuevo struct llamado FImpactData y lo añade a un array de FImpactData. Este struct es uno creado por mí que almacena los siguientes valores:
		FImpactData{
			AActor* ImpactedActor //El Actor que ha recibido daño
			FVector* ImpactPoint //La posición del Actor que ha producido el daño en el momento de hacerlo
			bool DecreasingImpact //Si ya han pasado X segundos desde que se produjo el daño esta variable se modifica a true y la opacidad del indicador se va devaluando de 1 hasta volver a llegar a 0
			float TimeToDecrease //Segundos que deben de pasar para convertir DecreasingImpact a true
			float ImpactValue //La opacidad que tendrá el indicador de daño. Mientras DecreasingImpact sea false será 1.f (su valor inicial) y cuando DecreasingImpact sea true irá bajando hasta llegar a 0
		}
	
	En el NativeTick de DamageIndicator, una vez que hay al menos 1 FImpactData en el array, se hace un bucle por cada FImpactData y se van modicando valores para ver si DecreasingImpact debería ser true o calcular el nuevo valor de ImpactValue.
	Además, en base al ImpactedActor y ImpactPoint se calcula la dirección del impacto y se añade a un nuevo array de FDirectionResult, dicho struct tiene los siguientes valores:
		FDirectionResult{
			EDirections Direction //Enum que indica la dirección del impacto (Up, Down, Right, Left)
			float ImpactValue //Valor de impacto dado por el FImpactData
		}
	Una vez rellenado ese array se hace un bucle de este y se dibujará el indicador de daño determinado por la dirección. Además, si hay 2 daños que tienen la misma dirección se dibujará el que mayor ImpactValue tenga.
	Finalmente se comprueba si algun FImpactData del array tiene el ImpactValue < 0 para eliminarlo del array.
	
- Pantalla Splash
	Aquí simplemente hay una pantalla en negro y pasados X segundos se funde y desaparece.
	Para ello, su NativeConstruct inicia un Timer el cual llama a la función RemoveScreen pasados 3 segundos.
	Dicha función inicia una animación llamada FadeAnim la cual la hicé por inspector y modifica la opacidad de la imagen de 1 a 0. Además, esta función también hace un Bind entre la animación y una función llamada OnFadeAnimFinished. Esta función se ejecutará cuando FadeAnim finalice.
	La función OnFadeAnimFinished modifica la Visibility del Widget a Collapsed y devuelve el input al jugador.
	Adicionalmente a esto, se hace un Bind entre la función  OnFadeAnimFinished y un Delegate del jugador en la función NativeConstruct del SplashScreen.
	Ese Delegate del jugador se ejecuta cuando este pulsa "F" y es una forma de hacer que el SplashScreen desaparezca sin necesidad de esperar los 3 segundos.
	
- Árbol de habilidades
	El SkillTree está dividido en tres ramas, dos de ellas con tres nodos y una con dos nodos. Las mejoras son funcionales y son de Valocidad (El personaje va más rápido. 3 nodos), Daño (Los disparos hacen más daño. 3 nodos) y Salto (El personaje tiene mayor fuerza de salto. 2 nodos).
	La forma en la que funciona es la siguiente: 
		Las mejoras parten de un UObject llamado SkillBase. Este script tiene una función abstracta llamada GetSkill. A partir de GetSkill heredan el resto de mejoras como SkillJump la cual implementa GetSkill aumentando la fuerza de salto del jugador.
		Para saber las mejoras que tiene el jugador, se gestionan todas desde un ActorComponent llamado SkillsComponent. Este componente tiene un array que se rellena desde inspector el cual relaciona el UObject de la habilidad con un GameplayTag.
		El componente tiene la función BuySkill la cual, si se tienen los suficientes SkillPoints, completa la compra. La compra se completa ejecutando el código del UObject de la habilidad asociado al GameplayTag de la habilidad que se quiere comprar.
	La forma en la que se pueden compra habilidades es desde el SkillTree pulsando en el Botón de la habilidad que se quiera comprar. El botón en sí es un Widget separado llamado SkillButton.
	Cada SkillButton tiene un DataAsset con los datos relacionados a la habilidad que se quiere comprar con ese botón. El DataAsset tiene los siguientes datos:
		SkillDataAsset{
			FGameplayTag SkillTag //Tag de la habilidad que se quiere comprar
			int SkillCost //Precio de la habilidad
			FText SkillDescription //Descripción de la habilidad
			UTexture2D* SkillTexture //Imagen que debe de tener el botón al comienzo
			UTexture2D* BoughtSkillTexture //Imagen que debe de tener el botón una vez se haya comprado la habilidad
		}
	Para comprar una habilidad hay que mantener pulsado el botón durante X segundos. Se comienzan a contar los segundos con el evento OnPressed y se dejan de contar con el evento OnReleased.
	OnPressed modifica un bool del SkillButton llamado BuyingSkill a true. Mientras esté en true un contador se va aumentado. Si dicho contador llega a un valor X se llamará a la función BuySkill, sin embargo, si se suelta el botón y se llama a la función ejecutada por el Delegate OnReleased, la variable BuyingSkill será false y se cancelará el intento de compra.
	Para mostrar feedback visual del proceso de compra se va aumentando el percentage de un ProgressBar dentro del SkillButton.
	La función BuySkill accede al SkillsComponent del jugador y le pasa su DataAsset el cual contiene el GameplayTag y el SkillCost para comprobar si se puede comprar la habilidad y saber la habilidad que se pretende comprar.
	Si la compra falla (el precio de la habilidad es mayor que los puntos actuales) sucederá una animación de parpadeo entre el color base y el rojo de los SkillPoints para mostrar feedback de que ese ha sido el motivo de la compra fallida.
	Por otro lado, si se completa la compra (Además de ejecutar el código asociado a la habilidad comprada) el botón actual cambia su imagen a la de BoughtSkillTexture de su DataAsset. Además, se desactiva y activa el SkillButton siguiente de su rama si es que hay más.
	Además de esto, si se mantiene el cursor por más de X segundos sobre el botón sin hacer click para comprar la habilidad aparece un cartel para mostrar información de la habilidad.
	El Delegate OnHovered del SkillButton inicia un Timer de X segundos, si este se completa aparece en la posición del ratón en la pantalla un nuevo Widget llamado SkillDataPanel. Este Widget recibe el USkillDataAsset del botón. Con el DataAsset obtiene el texto de la descripción de la habilidad y su precio y los indica en pantalla.
	Si se deja de mantener el cursor sobre el botón (Delegate de UnHovered del SkillButton) o si se comienza a comprar la habilidad (Delegate de OnPressed del SkillButton), el SkillDataPanel desaparece.