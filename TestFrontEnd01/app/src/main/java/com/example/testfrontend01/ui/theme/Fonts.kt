package com.example.testfrontend01.ui.theme

import androidx.compose.ui.text.font.Font
import androidx.compose.ui.text.font.FontFamily
import androidx.compose.material3.Typography
import androidx.compose.ui.text.TextStyle
import androidx.compose.ui.text.font.FontWeight
import com.example.testfrontend01.R // Import the R class for your resources

// Define the Inter font family from the font resource
val interFontFamily = FontFamily(
    Font(R.font.inter) // Make sure this matches the path of your font resource
)

// Create a custom Typography object using Inter font
val customTypography = Typography(
    displayLarge = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Bold),
    displayMedium = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Bold),
    displaySmall = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    headlineLarge = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    headlineMedium = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    headlineSmall = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    titleLarge = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    titleMedium = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    titleSmall = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    bodyLarge = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    bodyMedium = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    bodySmall = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    labelLarge = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    labelMedium = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal),
    labelSmall = TextStyle(fontFamily = interFontFamily, fontWeight = FontWeight.Normal)
)
