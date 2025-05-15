package com.example.testfrontend01

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.BorderStroke
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Spacer
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.height
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
import androidx.compose.foundation.lazy.grid.GridCells
import androidx.compose.foundation.lazy.grid.LazyVerticalGrid
import androidx.compose.foundation.lazy.grid.items
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Menu
import androidx.compose.material.icons.outlined.Add
import androidx.compose.material3.AlertDialog
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.CenterAlignedTopAppBar
import androidx.compose.material3.DrawerState
import androidx.compose.material3.DrawerValue
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.FloatingActionButton
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.ModalNavigationDrawer
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.TextButton
import androidx.compose.material3.TextField
import androidx.compose.material3.TopAppBarDefaults
import androidx.compose.material3.rememberDrawerState
import androidx.compose.material3.rememberTopAppBarState
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateListOf
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.draw.clip
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.example.testfrontend01.ui.theme.customTypography
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.launch


data class Terrarium(var name: String, var description: String)

// Global collection for Terrariums
val collection = mutableStateListOf<Terrarium>()

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            collection.add(Terrarium("n1", "d1"))
            collection.add(Terrarium("n2", "d2"))
            // Call a composable function that builds the UI
            TerrariumApp()
        }
    }
}

@Composable
fun TerrariumApp() {
    val showPopupMenu = remember { mutableStateOf(false) } // Boolean state
    val name = remember { mutableStateOf("") } // String state
    val description = remember { mutableStateOf("") } // String state

    val drawerState = rememberDrawerState(DrawerValue.Closed)
    val coroutineScope = rememberCoroutineScope()

    Scaffold(
        containerColor = Color.White,  // Set the background color for the entire screen
        topBar = {

            TerrariumTopAppBar(drawerState, coroutineScope)

        },
        floatingActionButton = {
            FloatingActionButton(
                onClick = { showPopupMenu.value = true },
                containerColor = Color.DarkGray, // Set the background color of FAB
                contentColor = Color.White, // Set the color of the icon inside FAB
            ) {
                Icon(Icons.Outlined.Add, contentDescription = "Add Terrarium")
            }
        },

    ) { innerPadding ->
        ModalNavigationDrawer(
            drawerState = drawerState,
            drawerContent = {
                Column(
                    modifier = Modifier.fillMaxSize(),
                    verticalArrangement = Arrangement.Center
                ) {
                    Text("Home", style = MaterialTheme.typography.headlineSmall, modifier = Modifier.padding(16.dp))
                    Text("About", style = MaterialTheme.typography.headlineSmall, modifier = Modifier.padding(16.dp))
                    Text("Services", style = MaterialTheme.typography.headlineSmall, modifier = Modifier.padding(16.dp))
                    Text("Contact", style = MaterialTheme.typography.headlineSmall, modifier = Modifier.padding(16.dp))
                }
            },
            content = {
                MaterialTheme(typography = customTypography) {
                    Column(modifier = Modifier.padding(innerPadding)) {
                        // Display the popup dialog if showPopupMenu is true
                        if (showPopupMenu.value) {
                            AlertDialog(
                                onDismissRequest = { showPopupMenu.value = false },
                                title = { Text("Add New Terrarium") },
                                text = {
                                    Column {
                                        TextField(
                                            value = name.value,
                                            onValueChange = { name.value = it },
                                            label = { Text("Name") })
                                        TextField(
                                            value = description.value,
                                            onValueChange = { description.value = it },
                                            label = { Text("Description") })
                                    }
                                },
                                confirmButton = {
                                    TextButton(onClick = {
                                        if (name.value.isNotEmpty() && description.value.isNotEmpty()) {
                                            collection.add(Terrarium(name.value, description.value))
                                            showPopupMenu.value = false
                                        }
                                    }) {
                                        Text("Add")
                                    }
                                },
                                dismissButton = {
                                    TextButton(onClick = { showPopupMenu.value = false }) {
                                        Text("Cancel")
                                    }
                                }
                            )
                        }
                        LazyVerticalGrid(
                            columns = GridCells.Fixed(2),
                            modifier = Modifier.fillMaxSize()
                        ) {
                            items(collection) { terrarium ->
                                TerrariumCard(terrarium)
                            }
                        }
                    }
                }
                // Content of your screen goes here
            }
        )
    }
}

@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun TerrariumTopAppBar(    drawerState: DrawerState,
                           coroutineScope: CoroutineScope   ) {
    // Creating a pinned scroll behavior for the TopAppBar
    val scrollBehavior = TopAppBarDefaults.pinnedScrollBehavior(rememberTopAppBarState())

    // The actual AppBar with navigation and actions
    CenterAlignedTopAppBar(
        modifier = Modifier.padding(top = 16.dp),
        colors = TopAppBarDefaults.centerAlignedTopAppBarColors(containerColor = Color.White),
        title = {
            Text(
                "myFlora", // Title of your AppBar
                maxLines = 1,
                style = MaterialTheme.typography.displaySmall.copy(fontWeight = FontWeight.ExtraBold)
            )
        },
        navigationIcon = {
            IconButton( onClick = {
                coroutineScope.launch { drawerState.open() }
            } ) {  // Trigger Drawer Open
                Icon(
                    imageVector = Icons.Filled.Menu,
                    contentDescription = "Open Drawer"
                )
            }
        },
        scrollBehavior = scrollBehavior // Apply scroll behavior if needed
    )
}

@Composable
fun TerrariumCard(terrarium: Terrarium) {
    Card(
        modifier = Modifier
            .padding(16.dp)
            .width(150.dp) // Fixed width
            .height(200.dp), // Fixed height
        shape = RoundedCornerShape(16.dp), // Rounded corners
        elevation = CardDefaults.cardElevation(4.dp),
        border = BorderStroke(1.dp, Color.White), // Thin border
        colors = CardDefaults.cardColors(containerColor = Color.White)
    ) {
        Box(
            modifier = Modifier.fillMaxSize() // Make the Box take up all available space
        ) {
            Column(
                modifier = Modifier
                    .align(Alignment.BottomCenter) // Aligns content at the bottom center
                    .padding(12.dp), // Padding around the text
            horizontalAlignment = Alignment.CenterHorizontally

            ) {
                Spacer(modifier = Modifier.height(6.dp)) // Add some space between name and description
                Image(
                    painter = painterResource(id = R.drawable.jar1),
                    contentDescription = "Terrarium Image",
                    modifier = Modifier
                        .fillMaxWidth()
                        .height(100.dp) // You can adjust height as needed
                        .clip(RoundedCornerShape(topStart = 16.dp, topEnd = 16.dp)),
                    contentScale = ContentScale.Fit
                )
                Text(
                    text = terrarium.name,
                    style = MaterialTheme.typography.headlineLarge.copy(fontWeight = FontWeight.ExtraBold),
                    textAlign = TextAlign.Center,
                    )
                Spacer(modifier = Modifier.height(6.dp)) // Add some space between name and description
                Text(
                    text = terrarium.description,
                    style = MaterialTheme.typography.bodySmall.copy(fontWeight = FontWeight.Thin),
                    textAlign = TextAlign.Center,
                )
            }
        }
    }
}

@Preview(showBackground = true)
@Composable
fun DefaultPreview() {
    TerrariumApp()
}