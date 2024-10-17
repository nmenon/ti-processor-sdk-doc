ARG BASE_DISTRO=debian:stable-slim
FROM $BASE_DISTRO

ARG USER_UID=1000

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update \
	&& apt-get install -y --no-install-recommends \
		python3-sphinx python3-pip \
	&& echo "**** cleanup ****" \
	&& apt-get autoremove \
	&& apt-get clean \
	&& rm -rf \
		/tmp/* \
		/var/lib/apt/lists/* \
		/var/tmp/* \
		/var/log/*

RUN --mount=type=bind,source=requirements.txt,target=/tmp/requirements.txt \
		python3 -m pip install --root-user-action=ignore --break-system-packages --upgrade pip setuptools && \
		python3 -m pip install --root-user-action=ignore --break-system-packages -r /tmp/requirements.txt

# Publish the source repository
LABEL org.opencontainers.image.source https://github.com/TexasInstruments/processor-sdk-doc

RUN echo "**** create user and make our folders ****" \
	&& useradd -u $USER_UID -U -d /config -s /bin/false user \
	&& usermod -G users user \
	&& mkdir /workdir && chown user:user /workdir \
	&& mkdir /config && chown user:user /config

ENTRYPOINT ["/init"]
CMD ["/usr/bin/bash"]
VOLUME /workdir
WORKDIR /workdir
