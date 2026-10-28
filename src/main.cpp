#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

const char *nomeRedeWiFi = "SENAI_EDUCACIONAL";
const char *SenhaRedeWiFi = "info@134";
String enderecoAPI = "https://pokeapi.co/api/v2/pokemon/";
// TODO Nao esquecer de contatenar o id do pokemon na url

String pedirDadosPokemonAPI(unsigned int IdPesquisaPokemon);
void pokemonInfo(int Id);

void setup()
{
  Serial.begin(115200);

  //* Conexao com WiFi muito crua e rudimentar
  Serial.println();
  WiFi.begin(nomeRedeWiFi, SenhaRedeWiFi);

  Serial.print("Conectando ao WiFi.");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\n\rConectado com secesso!\n\r");

  randomSeed(esp_random());
}

void loop()
{
  int numeroPseudoAleatorio = random(1, 1001);
  pokemonInfo(numeroPseudoAleatorio);
  delay(5000);
}

void pokemonInfo(int Id)
{
  String jsonPokemon = (pedirDadosPokemonAPI(Id));

  JsonDocument docPokemon;
  JsonDocument filtroPokemon;
  filtroPokemon["name"] = true;
  filtroPokemon["id"] = true;
  filtroPokemon["abilities"][0]["ability"]["name"] = true;
  filtroPokemon["types"][0]["type"]["name"] = true;
  filtroPokemon["forms"][0]["name"] = true;

  DeserializationError codigoStatusJson = deserializeJson(docPokemon, jsonPokemon, DeserializationOption::Filter(filtroPokemon));

  String nomePokemon = docPokemon["name"].as<String>();
  int IdPokemon = docPokemon["id"].as<int>();

  JsonArrayConst habilidades = docPokemon["abilities"];
  JsonArrayConst tipos = docPokemon["types"];
  JsonArrayConst formas = docPokemon["forms"];

  Serial.printf("\n\r--------------------------------------------\n\r");
  Serial.printf("Nome do Pokemon: %s", docPokemon["name"]);
  Serial.printf("Id: %d\n\r", IdPokemon);

  Serial.print("Habilidades: ");
  for (int i = 0; i < habilidades.size(); i++)
  {
    Serial.print(habilidades[i]["abilities"]["name"].as<const char *>());
    Serial.print(
        (i < habilidades.size() - 1)
            ? ", "
            : ".\n\r");
  }

  Serial.print("Tipos: ");
  for (int i = 0; i < tipos.size(); i++)
  {
    Serial.print(tipos[i]["type"]["name"].as<const char *>());
    Serial.print(
        (i < tipos.size() - 1)
            ? ", "
            : ".\n\r");
  }

  Serial.print("Formas: ");
  for (int i = 0; i < formas.size(); i++)
  {
    Serial.print(formas[i]["name"].as<const char *>());
    Serial.print(
        (i < formas.size() - 1)
            ? ", "
            : ".\n\r");
  }
}

String pedirDadosPokemonAPI(unsigned int IdPesquisaPokemon)
{
  String resposta = "";

  String enderecoCompletoAPI = enderecoAPI + IdPesquisaPokemon + "/";

  Serial.printf("Endereco a ser pesquisado: %s\n\r", enderecoCompletoAPI.c_str());

  HTTPClient httpPokemon;
  httpPokemon.begin(enderecoCompletoAPI);

  int HTTPCode = httpPokemon.GET();

  if (HTTPCode != 200)
  {
    resposta = "Erro ao consultar a url:" + HTTPCode;
    return resposta;
  }

  resposta = httpPokemon.getString();

  return resposta;
}