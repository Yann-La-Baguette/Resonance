package com.example.resonance_android_studio

import java.nio.ByteBuffer
import java.nio.ByteOrder
import android.content.Context
import android.media.AudioFormat
import android.media.AudioManager
import android.media.AudioTrack
import android.util.Log
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import java.net.DatagramPacket
import java.net.DatagramSocket
import java.net.InetAddress
import java.net.InetSocketAddress

class UDPReceiver(
    private val context: Context,
    private val ipAddress: String,
    private var port: Int = 5000
) {
    private val sampleRate = 48000
    private val channelConfig = AudioFormat.CHANNEL_OUT_STEREO
    private val audioFormat = AudioFormat.ENCODING_PCM_FLOAT
    private val udpBufferSize = 8192
    private val bufferSize = AudioTrack.getMinBufferSize(sampleRate, channelConfig, audioFormat)
        .coerceAtLeast(udpBufferSize)

    private var socket: DatagramSocket? = null

    private val audioTrack = AudioTrack(
        AudioManager.STREAM_MUSIC,
        sampleRate,
        channelConfig,
        audioFormat,
        bufferSize,
        AudioTrack.MODE_STREAM
    )

    init {
        require(port in 0..65535) { "Port invalide à l'initialisation : $port" }
        Log.d("UDPReceiver", "UDPReceiver initialisé avec IP=$ipAddress et port=$port")
    }

    fun setPort(newPort: Int) {
        if (newPort in 0..65535) {
            port = newPort
        } else {
            throw IllegalArgumentException("Port invalide : $newPort")
        }
    }

    fun startListening(onAudioReceiving: (Boolean) -> Unit) {
        val listenIp = "0.0.0.0"
        try {
            val inetAddress = InetAddress.getByName(listenIp)
            socket = DatagramSocket(null).apply {
                reuseAddress = true
                bind(InetSocketAddress(inetAddress, 5000))
            }

            val audioManager = context.getSystemService(Context.AUDIO_SERVICE) as AudioManager
            audioManager.mode = AudioManager.MODE_NORMAL
            audioManager.isSpeakerphoneOn = true
            audioTrack.play()
            onAudioReceiving(true)

            CoroutineScope(Dispatchers.IO).launch {
                val buffer = ByteArray(udpBufferSize)
                while (socket != null && !socket!!.isClosed) {
                    val packet = DatagramPacket(buffer, buffer.size)
                    try {
                        socket!!.receive(packet)

                        val floatBuffer = ByteBuffer.wrap(packet.data, 0, packet.length)
                            .order(ByteOrder.LITTLE_ENDIAN)
                            .asFloatBuffer()
                        val floatArray = FloatArray(floatBuffer.remaining())
                        floatBuffer.get(floatArray)

                        audioTrack.write(floatArray, 0, floatArray.size, AudioTrack.WRITE_BLOCKING)
                        Log.d("UDPReceiver", "Audio reçu (${packet.length} octets)")
                    } catch (e: Exception) {
                        Log.e("UDPReceiver", "Erreur de réception : ${e.message}")
                        break
                    }
                }
            }
        } catch (e: Exception) {
            e.printStackTrace()
            onAudioReceiving(false)
            Log.e("UDPReceiver", "Erreur lors de la liaison du socket : ${e.message}")
        }
    }

    fun stopListening() {
        socket?.close()
        audioTrack.stop()
        Log.d("UDPReceiver", "Socket fermé et audio arrêté")
    }
}