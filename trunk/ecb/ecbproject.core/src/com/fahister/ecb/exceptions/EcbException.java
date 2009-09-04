package com.fahister.ecb.exceptions;

public class EcbException extends RuntimeException {
	private static final long serialVersionUID = 1L;

	public EcbException(String messageCode) {
		super(messageCode);
	}

	public EcbException(String messageCode, Exception exception) {
		super(messageCode, exception);
	}
}