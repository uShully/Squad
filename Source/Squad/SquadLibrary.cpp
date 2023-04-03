// Fill out your copyright notice in the Description page of Project Settings.

#include "SquadLibrary.h"

void USquadLibrary::ChangeLocalizationCulture(ELocalizationCulture culture)
{
	switch (culture) {
	case EN:
		FInternationalization::Get().SetCurrentCulture(TEXT("en"));
		break;
	case KO:
		FInternationalization::Get().SetCurrentCulture(TEXT("ko-KR"));
		break;
	case AR:
		FInternationalization::Get().SetCurrentCulture(TEXT("ar"));
		break;
	case BG:
		FInternationalization::Get().SetCurrentCulture(TEXT("bg-BG"));
		break;
	case DA:
		FInternationalization::Get().SetCurrentCulture(TEXT("da"));
		break;
	case NL:
		FInternationalization::Get().SetCurrentCulture(TEXT("nl"));
		break;
	case FI:
		FInternationalization::Get().SetCurrentCulture(TEXT("fi"));
		break;
	case FR:
		FInternationalization::Get().SetCurrentCulture(TEXT("fr"));
		break;
	case DE:
		FInternationalization::Get().SetCurrentCulture(TEXT("de"));
		break;
	case ID:
		FInternationalization::Get().SetCurrentCulture(TEXT("id"));
		break;
	case IT:
		FInternationalization::Get().SetCurrentCulture(TEXT("it"));
		break;
	case LA:
		FInternationalization::Get().SetCurrentCulture(TEXT("la"));
		break;
	case MS:
		FInternationalization::Get().SetCurrentCulture(TEXT("ms"));
		break;
	case NO:
		FInternationalization::Get().SetCurrentCulture(TEXT("no"));
		break;
	case PT_BR:
		FInternationalization::Get().SetCurrentCulture(TEXT("pt-BR"));
		break;
	case PT:
		FInternationalization::Get().SetCurrentCulture(TEXT("pt"));
		break;
	case RU:
		FInternationalization::Get().SetCurrentCulture(TEXT("ru"));
		break;
	case ES:
		FInternationalization::Get().SetCurrentCulture(TEXT("es"));
		break;
	case SV:
		FInternationalization::Get().SetCurrentCulture(TEXT("sv"));
		break;
	case TR:
		FInternationalization::Get().SetCurrentCulture(TEXT("tr"));
		break;
	case VI:
		FInternationalization::Get().SetCurrentCulture(TEXT("vi"));
		break;
	case JA_JP:
		FInternationalization::Get().SetCurrentCulture(TEXT("ja-JP"));
		break;
	case ZH_HANS:
		FInternationalization::Get().SetCurrentCulture(TEXT("zh-Hans"));
		break;
	case ZH_HANT:
		FInternationalization::Get().SetCurrentCulture(TEXT("zh-Hant"));
		break;
	default:
		break;
	}
}

