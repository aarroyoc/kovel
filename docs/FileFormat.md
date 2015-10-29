# Kovel Voxel Format

Kovel is binary JSON (BSON)

Mime-Type: application/x-kovel-voxel
Extension: .kvl

```json
{
	"metadata" : {
		"name" : "",
		"creator" : "",
		"date" : "",
		"version" : ""
	},
	"materials" : {
		"MATERIAL_ID" : {
			"red" : "",
			"green" : "",
			"blue" : "",
			"opacity" : "",
			"texture" : "Base64 encoded texture"
		}
	},
	"voxels" : [ //X
		[		// Y
			[{
				/* Current */
				"material" : "MATERIAL_ID",
				/* In a future */
				"material" : {
					"top" : "MATERIAL_ID",
					"bottom": "MATERIAL_ID",
					"front" : "MATERIAL_ID",
					"back" : "MATERIAL_ID",
					"left" : "MATERIAL_ID",
					"right" : "MATERIAL_ID"
				},
				/* optional */
				"custom_mesh" : CustomMeshObject or INTEGER /* To be implemented */
			}] //Z
		]
	]

}
```
