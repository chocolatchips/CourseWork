
let baseURL = "https://pokeapi.co/api/v2/pokemon/";
let moveURL = "https://pokeapi.co/api/v2/move/";
let typeURL = "https://pokeapi.co/api/v2/type/";

$("#searchPK").keypress(function(event) {
	if(event.keyCode === 13) {
		$("#searchButton").click();
	}
});

function hideAll(){
	$("#battle").hide();
	$("#userAttributes").hide();
	$("#oppAttributes").hide();
	$("#oppMoves").hide();
	//alert("This page is able to simulate a Pokemon battle with the user being able to specifically or randomly select a Pokemon.\n Currently only moves that deal damage are currently supported.\n Moves are randomly assigned based on all possible moves that can be used by the Pokemon.");
}

$("#userMove1Name").hover(function(){$(this).css("background-color", changeColour($("#userMove1Type").text()));}, function(){$(this).css("background-color", "white");});
$("#userMove2Name").hover(function(){$(this).css("background-color", changeColour($("#userMove2Type").text()));}, function(){$(this).css("background-color", "white");});
$("#userMove3Name").hover(function(){$(this).css("background-color", changeColour($("#userMove3Type").text()));}, function(){$(this).css("background-color", "white");});
$("#userMove4Name").hover(function(){$(this).css("background-color", changeColour($("#userMove4Type").text()));}, function(){$(this).css("background-color", "white");});


function searchPokemon(){
	if ($("#searchPK").val().length == 0){
		genRandBattle();
		return;
	}
	
	var pkURL = baseURL + $("#searchPK").val().toLowerCase() + "/";
	$("#battle").show();
	
	fetch(pkURL).then(function(response) {
		if (response.status == 404){
			genRandBattle();
			return;
		}
	});
	$.getJSON(pkURL, function(data){
		
		userPK = data.species.name;
		addUserPhotos(data);
		setTypes("user", data);
		$("body").css("background-color", changeColour(data.types[data.types.length-1].type.name));
		userPK = userPK.charAt(0).toUpperCase() + userPK.slice(1);
		$("#userPkName").html(userPK);
		setStats("user", data);
		setMove("user", data);
	});
	genOppPoke();
}

function genRandBattle(){
	
	$("#battle").show();
	var userPK, oppPK;
	
	var userRand = Math.floor(Math.random()*649); //Restricted based on pokemon that have a back image
	var userURL = baseURL + userRand + "/";
	$.getJSON(userURL, function(data){
		userPK = data.species.name;
		addUserPhotos(data);
		setTypes("user", data);
		$("body").css("background-color", changeColour(data.types[data.types.length-1].type.name));
		userPK = userPK.charAt(0).toUpperCase() + userPK.slice(1);
		$("#userPkName").html(userPK);
		setStats("user", data);
		setMove("user", data);
		
		
	});
	
	genOppPoke();
}
function genOppPoke(){
	var oppRand = Math.floor(Math.random()*801);
	var oppURL = baseURL + oppRand + "/";
	$.getJSON(oppURL, function(data){
		setTypes("opp", data);
		oppPK = data.species.name;
		addOppPhotos(data);
		oppPK = oppPK.charAt(0).toUpperCase() + oppPK.slice(1);
		$("#oppPkName").html(oppPK);
		setStats("opp", data);
		setMove("opp", data);
		//$("#oppMove1Name").show();
		//$("#oppMove2Name").show();
		//$("#oppMove3Name").show();
		//$("#oppMove4Name").show();
	});
}
function playerAttack(player, against, moveNum){
	var damage;
	var url = typeURL+$("#"+player+"Move"+moveNum+"Type").text()+"/";
	var multiplier = 1;
	if($("#"+player+"Move"+moveNum+"DamageClass").text() == "physical"){
		$.getJSON(url, function(data){
			for (var i = 0; i < data.damage_relations.double_damage_from.length; i++){
				if($("#"+against+"Type1").text() === data.damage_relations.double_damage_from[i].name.text()){
					multiplier = multiplier * 2;
				}
				if($("#"+against+"Type2").text() === data.damage_relations.double_damage_from[i].name.text()){
					multiplier = multiplier * 2;
				}
			}
			for (var i = 0; i < data.damage_relations.half_damage_from.length; i++){
				if($("#"+against+"Type1").text() === data.damage_relations.half_damage_from[i].name.text()){
					multiplier = multiplier * 0.5;
				}
				if($("#"+against+"Type2").text() === data.damage_relations.half_damage_from[i].name.text()){
					multiplier = multiplier * 0.5;
				}
			}
			for (var i = 0; i < data.damage_relations.no_damage_from.length; i++){
				if($("#"+against+"Type1").text() === data.damage_relations.no_damage_from[i].name.text()){
					multiplier = multiplier * 0;
				}
				if($("#"+against+"Type2").text() === data.damage_relations.no_damage_from[i].name.text()){
					multiplier = multiplier * 0;
				}
			}
			damage = ($("#"+player+"Move"+moveNum+"Power").text() * multiplier) - $("#"+against+"defence").text();
		if(damage <= 0){
			damage = 1;
		}
		});
	}
	else if ($("#"+player+"Move"+moveNum+"DamageClass").text() == "special"){
		$.getJSON(url, function(data){
			for (var i = 0; i < data.damage_relations.double_damage_from.length; i++){
				if($("#"+against+"Type1").text() === data.damage_relations.double_damage_from[i].name.text()){
					multiplier = multiplier * 2;
				}
				if($("#"+against+"Type2").text() === data.damage_relations.double_damage_from[i].name.text()){
					multiplier = multiplier * 2;
				}
			}
			for (var i = 0; i < data.damage_relations.half_damage_from.length; i++){
				if($("#"+against+"Type1").text() === data.damage_relations.half_damage_from[i].name.text()){
					multiplier = multiplier * 0.5;
				}
				if($("#"+against+"Type2").text() === data.damage_relations.half_damage_from[i].name.text()){
					multiplier = multiplier * 0.5;
				}
			}
			for (var i = 0; i < data.damage_relations.no_damage_from.length; i++){
				if($("#"+against+"Type1").text() === data.damage_relations.no_damage_from[i].name.text()){
					multiplier = multiplier * 0;
				}
				if($("#"+against+"Type2").text() === data.damage_relations.no_damage_from[i].name.text()){
					multiplier = multiplier * 0;
				}
			}
			damage = ($("#"+player+"Move"+moveNum+"Power").text() * multiplier) - $("#"+against+"special-defence").text();
			if(damage <= 0){
			damage = 1;
			}
		});
	}
	else{
		alert($("#"+player+"Move"+moveNum+"Name").html()+" is currently not supported");
	}
	
	
}

function battleSequence(moveNum){
	var firstMove = $("#userSpeed").text() - $("#oppSpeed").text();
	var oppMove = Math.ceil(Math.random()*4);
	if (firstMove >= 0){
		playerAttack("user", "opp", moveNum);
		//playerAttack("opp", "user", oppMove);
	}
	else{
		//playerAttack("opp", "user", oppMove);
		playerAttack("user", "opp", moveNum);
	}
	
	
	
}
function setMoveStats(url, player, moveNum){
	
	$.getJSON(url, function(data){
		
		$("#"+player+"Move"+moveNum+"Acc").html(data.accuracy);
		$("#"+player+"Move"+moveNum+"Power").html(data.power);
		$("#"+player+"Move"+moveNum+"Type").html(data.type.name);
		$("#"+player+"Move"+moveNum+"DamageClass").html(data.damage_class.name);
		if(data.stat_changes.length !== 0){
			$("#"+player+"Move"+moveNum+"Effect").html(data.stat_changes[0].stat.name);
			$("#"+player+"Move"+moveNum+"EffectVal").html(data.stat_changes[0].change);
		}
		else{
			$("#"+player+"Move"+moveNum+"Effect").html("none");
			$("#"+player+"Move"+moveNum+"EffectVal").html("none");
		}
		$("#"+player+"Move"+moveNum+"Type").html(data.type.name);
		$("#"+player+"Move"+moveNum+"Target").html(data.target.name);
		
	});
}
function setMove(player, data){
	var moveLength = data.moves.length;
	var move1, move2, move3, move4;
	move1 = Math.floor(Math.random()*moveLength);
	if(moveLength > 1){
		do{
			move2 = Math.floor(Math.random()*moveLength);
		}while(move2 === move1);
		$("#"+player+"Move2Name").html(data.moves[move2].move.name);
		move2URL = moveURL+data.moves[move2].move.name+"/";
		setMoveStats(move2URL, player, 2);
		if(moveLength > 2){
			do{
				move3 = Math.floor(Math.random()*moveLength);
			}while(move3 === move1 || move3 === move2);
			$("#"+player+"Move3Name").html(data.moves[move3].move.name);
			move3URL = moveURL+data.moves[move3].move.name+"/";
			setMoveStats(move3URL, player, 3);
			if(moveLength > 3){
				do{
					move4 = Math.floor(Math.random()*moveLength);
				}while(move4 === move1 || move4 === move2 || move4 === move3);
				$("#"+player+"Move4Name").html(data.moves[move4].move.name);
				move4URL = moveURL+data.moves[move4].move.name+"/";
				setMoveStats(move4URL, player, 4);
			}
			else{
				$("#"+player+"Move4Name").hide();
			}
		}
		else {
			$("#"+player+"Move3Name").hide();
			$("#"+player+"Move4Name").hide();
		}
	}
	else{
		$("#"+player+"Move2Name").hide();
		$("#"+player+"Move3Name").hide();
		$("#"+player+"Move4Name").hide();
	}
	
	$("#"+player+"Move1Name").html(data.moves[move1].move.name);
	move1URL = moveURL+data.moves[move1].move.name+"/";
	setMoveStats(move1URL, player, 1);
}
function setTypes(player, data){
	if(data.types.length > 1){
			$("#"+player+"Type1").html(data.types[0].type.name);
			$("#"+player+"Type2").html(data.types[1].type.name);
		}
		else{
			$("#"+player+"Type1").html(data.types[0].type.name);
		}
}
function setStats(player, data){
	$("#"+player+"Speed").html(data.stats[0].base_stat);
	$("#"+player+"special-defence").html(data.stats[1].base_stat);
	$("#"+player+"special-attack").html(data.stats[2].base_stat);
	$("#"+player+"defence").html(data.stats[3].base_stat);
	$("#"+player+"attack").html(data.stats[4].base_stat);
	$("#"+player+"Current").html(data.stats[5].base_stat);
	$("#"+player+"HealthMax").html(data.stats[5].base_stat);
}
function addUserPhotos(data){

	var	shiny = Math.floor(Math.random()*8192);
	if ((shiny === 0)  && (data.sprites.back_shiny !== null)){
		$("#user_def").attr('src', data.sprites.back_shiny);
	}
	else{
		$("#user_def").attr('src', data.sprites.back_default);
	}
	$("#user_def").show();
}
function addOppPhotos(data){
	shiny = Math.floor(Math.random()*8192);
	if ((shiny === 0)  && (data.sprites.back_shiny !== null)){
		$("#opp_def").attr('src', data.sprites.front_shiny);
	}
	else{
		$("#opp_def").attr('src', data.sprites.front_default);
	}
	$("#opp_def").show();
}
function changeColour(data){
	var type = data;
	var colour;
	switch(type){
		case "fighting":
			colour = "#C03028";
			break;
		case "flying":
			colour = "#A890F0";
			break;
		case "poison":
			colour = "#A040A0";
			break;
		case "rock":
			colour = "#B8A038";
			break;
		case "ground":
			colour = "#E0C068";
			break;
		case "bug":
			colour = "#A8B820";
			break;
		case "ghost":
			colour = "#705898";
			break;
		case "steel":
			colour = "B8B8D0";
			break;
		case "fire":
			colour = "F08030";
			break;
		case "water":
			colour = "6890F0";
			break;
		case "grass":
			colour = "78C850";
			break;
		case "electric":
			colour = "F8D030";
			break;
		case "psychic":
			colour = "F85888";
			break;
		case "ice":
			colour = "98D8D8";
			break;
		case "dragon":
			colour = "7038F8";
			break;
		case "dark":
			colour = "705848";
			break;
		case "fairy":
			colour = "EE99AC";
			break;
		default:
			colour = "white";
			break;
	}
	return colour;
}
