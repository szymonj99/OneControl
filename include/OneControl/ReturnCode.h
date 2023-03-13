#pragma once

#include <string>

namespace oc
{
	// TODO: This could be split into more enums I think.
	enum class ReturnCode
	{
		Success = 0,
		HandshakeFailed,
		VersionMismatch,
		InvalidTypeString,
		NoPacketReceived, // This is not exactly 100% accurate. TODO: Make this more accurate.
		EncryptionStateMismatch,
		ShowHelpMenu,
		ParserError,
		ValidationError,
		InvalidIPAddress,
		FailedSendingPacket,
		CheckParserOutput,
		NotAbleToBindOnPort,
		NotAbleToAcceptClient,
		NotAbleToConnectToServer
	};

	// TODO: Figure out if this is actaully needed at all.
	static std::string ReturnCodeToString(const oc::ReturnCode& code)
	{
		switch (code)
		{
		case oc::ReturnCode::Success:
			return "success";
		case oc::ReturnCode::HandshakeFailed:
			return "handshake failed";
		case oc::ReturnCode::VersionMismatch:
			return "version mismatch";
		case oc::ReturnCode::InvalidTypeString:
			return "invalid type string for server or client";
		case oc::ReturnCode::NoPacketReceived:
			return "no packet received";
		case oc::ReturnCode::EncryptionStateMismatch:
			return "the client and server did not agree on a consistent encryption scheme, if any";
		case oc::ReturnCode::ShowHelpMenu:
			return "show help menu";
		case oc::ReturnCode::ParserError:
			return "encountered an error when parsing options";
		case oc::ReturnCode::ValidationError:
			return "encountered an error when validating parsed options";
		case oc::ReturnCode::InvalidIPAddress:
			return "the ip address provided is invalid";
		case oc::ReturnCode::FailedSendingPacket:
			return "sending the packet has not been successful";
		default:
			return "unknown";
		}
	}
}