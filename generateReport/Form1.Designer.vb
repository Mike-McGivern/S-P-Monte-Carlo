<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()>
Partial Class Form1
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()>
    Protected Overrides Sub Dispose(disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()>
    Private Sub InitializeComponent()
        btnRunReport = New Button()
        lblStatus = New Label()
        ProgressBar1 = New ProgressBar()
        chkIndex = New CheckBox()
        chkConstituents = New CheckBox()
        chkVolatility = New CheckBox()
        chkMomentum = New CheckBox()
        chkSector = New CheckBox()
        chkRisk = New CheckBox()
        chkLiquidity = New CheckBox()
        SuspendLayout()
        ' 
        ' btnRunReport
        ' 
        btnRunReport.Location = New Point(218, 144)
        btnRunReport.Name = "btnRunReport"
        btnRunReport.Size = New Size(302, 60)
        btnRunReport.TabIndex = 0
        btnRunReport.Text = "Generate Report"
        btnRunReport.UseVisualStyleBackColor = True
        ' 
        ' lblStatus
        ' 
        lblStatus.AutoSize = True
        lblStatus.Location = New Point(218, 207)
        lblStatus.Name = "lblStatus"
        lblStatus.Size = New Size(60, 25)
        lblStatus.TabIndex = 1
        lblStatus.Text = "Status"
        ' 
        ' ProgressBar1
        ' 
        ProgressBar1.Location = New Point(218, 235)
        ProgressBar1.Name = "ProgressBar1"
        ProgressBar1.Size = New Size(302, 34)
        ProgressBar1.TabIndex = 2
        ' 
        ' chkIndex
        ' 
        chkIndex.AutoSize = True
        chkIndex.Checked = True
        chkIndex.CheckState = CheckState.Checked
        chkIndex.Location = New Point(27, 109)
        chkIndex.Name = "chkIndex"
        chkIndex.Size = New Size(81, 29)
        chkIndex.TabIndex = 3
        chkIndex.Text = "Index"
        chkIndex.UseVisualStyleBackColor = True
        ' 
        ' chkConstituents
        ' 
        chkConstituents.AutoSize = True
        chkConstituents.Location = New Point(114, 109)
        chkConstituents.Name = "chkConstituents"
        chkConstituents.Size = New Size(137, 29)
        chkConstituents.TabIndex = 4
        chkConstituents.Text = "Constituents"
        chkConstituents.UseVisualStyleBackColor = True
        ' 
        ' chkVolatility
        ' 
        chkVolatility.AutoSize = True
        chkVolatility.Location = New Point(257, 109)
        chkVolatility.Name = "chkVolatility"
        chkVolatility.Size = New Size(105, 29)
        chkVolatility.TabIndex = 5
        chkVolatility.Text = "Volatility"
        chkVolatility.UseVisualStyleBackColor = True
        ' 
        ' chkMomentum
        ' 
        chkMomentum.AutoSize = True
        chkMomentum.Location = New Point(368, 109)
        chkMomentum.Name = "chkMomentum"
        chkMomentum.Size = New Size(132, 29)
        chkMomentum.TabIndex = 6
        chkMomentum.Text = "Momentum"
        chkMomentum.UseVisualStyleBackColor = True
        ' 
        ' chkSector
        ' 
        chkSector.AutoSize = True
        chkSector.Location = New Point(506, 109)
        chkSector.Name = "chkSector"
        chkSector.Size = New Size(88, 29)
        chkSector.TabIndex = 7
        chkSector.Text = "Sector"
        chkSector.UseVisualStyleBackColor = True
        ' 
        ' chkRisk
        ' 
        chkRisk.AutoSize = True
        chkRisk.Location = New Point(600, 109)
        chkRisk.Name = "chkRisk"
        chkRisk.Size = New Size(70, 29)
        chkRisk.TabIndex = 8
        chkRisk.Text = "Risk"
        chkRisk.UseVisualStyleBackColor = True
        ' 
        ' chkLiquidity
        ' 
        chkLiquidity.AutoSize = True
        chkLiquidity.Location = New Point(676, 109)
        chkLiquidity.Name = "chkLiquidity"
        chkLiquidity.Size = New Size(105, 29)
        chkLiquidity.TabIndex = 9
        chkLiquidity.Text = "Liquidity"
        chkLiquidity.UseVisualStyleBackColor = True
        ' 
        ' Form1
        ' 
        AutoScaleDimensions = New SizeF(10F, 25F)
        AutoScaleMode = AutoScaleMode.Font
        ClientSize = New Size(800, 450)
        Controls.Add(chkLiquidity)
        Controls.Add(chkRisk)
        Controls.Add(chkSector)
        Controls.Add(chkMomentum)
        Controls.Add(chkVolatility)
        Controls.Add(chkConstituents)
        Controls.Add(chkIndex)
        Controls.Add(ProgressBar1)
        Controls.Add(lblStatus)
        Controls.Add(btnRunReport)
        Name = "Form1"
        Text = "Form1"
        ResumeLayout(False)
        PerformLayout()
    End Sub

    Friend WithEvents btnRunReport As Button
    Friend WithEvents lblStatus As Label
    Friend WithEvents ProgressBar1 As ProgressBar
    Friend WithEvents chkIndex As CheckBox
    Friend WithEvents chkConstituents As CheckBox
    Friend WithEvents chkVolatility As CheckBox
    Friend WithEvents chkMomentum As CheckBox
    Friend WithEvents chkSector As CheckBox
    Friend WithEvents chkRisk As CheckBox
    Friend WithEvents chkLiquidity As CheckBox

End Class
