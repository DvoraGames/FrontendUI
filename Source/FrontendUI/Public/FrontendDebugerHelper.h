#pragma once

// namespace Debug foi criado para evitar conflitos
namespace Debug
{
	// Função responsavel por "Printar" na tela do jogo e no Console uma mensagem
	static void Print(const FString& Msg,int32 InKey = -1, const FColor& InColor = FColor::MakeRandomColor())
	{
		// Verifica se GEngine é valida
		if (GEngine)
		{
			// "Printa" a menssagem na tela
			GEngine->AddOnScreenDebugMessage(InKey, 7.f, InColor, Msg);
			
			// "Printa" a menssagem no Console
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}
}
