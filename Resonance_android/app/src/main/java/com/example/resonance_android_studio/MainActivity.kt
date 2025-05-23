package com.example.resonance_android_studio

import android.content.Context
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp
import androidx.core.view.WindowCompat
import com.example.resonance_android_studio.ui.theme.Resonance_Android_StudioTheme
import androidx.compose.foundation.layout.WindowInsets
import androidx.compose.foundation.layout.asPaddingValues
import androidx.compose.foundation.layout.systemBars
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Spacer
import androidx.compose.ui.Alignment
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.text.font.Font
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.ui.unit.sp
import androidx.compose.foundation.layout.height
import android.net.wifi.WifiManager
import android.util.Log

class MainActivity : ComponentActivity() {
    private var udpReceiver: UDPReceiver? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        WindowCompat.setDecorFitsSystemWindows(window, false)

        // Démarre l'écoute UDP dès l'ouverture
        udpReceiver = UDPReceiver(this, "0.0.0.0", 5000)
        udpReceiver?.startListening { isReceiving ->
            if (isReceiving) {
                Log.d("MainActivity", "Réception audio démarrée")
            } else {
                Log.e("MainActivity", "Échec du démarrage de la réception audio")
            }
        }

        setContent {
            Resonance_Android_StudioTheme {
                Scaffold(
                    modifier = Modifier
                        .fillMaxSize()
                        .padding(WindowInsets.systemBars.asPaddingValues())
                ) { innerPadding ->
                    HomeMenu(
                        modifier = Modifier.padding(innerPadding),
                        context = this
                    )
                }
            }
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        udpReceiver?.stopListening()
    }
}

@Composable
fun HomeMenu(modifier: Modifier = Modifier, context: Context) {
    val wifiManager = context.applicationContext.getSystemService(Context.WIFI_SERVICE) as WifiManager
    val deviceIpAddress = android.text.format.Formatter.formatIpAddress(wifiManager.connectionInfo.ipAddress)

    Column(
        modifier = modifier
            .fillMaxSize()
            .padding(16.dp),
        verticalArrangement = Arrangement.Center,
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(
            text = "RESONANCE",
            fontSize = 32.sp,
            fontFamily = FontFamily(Font(R.font.boldonse_regular)),
            modifier = Modifier
                .align(Alignment.CenterHorizontally)
                .padding(top = 32.dp)
        )
        Spacer(modifier = Modifier.height(16.dp))
        Text(
            text = "Votre adresse IP : $deviceIpAddress",
            fontSize = 14.sp,
            color = Color.Gray,
            modifier = Modifier.align(Alignment.Start).padding(top = 8.dp)
        )
    }
}