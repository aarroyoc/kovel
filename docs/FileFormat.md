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
			"color" : "",
			"opacity" : "",
			"texture" : "Base64 encoded texture"
		}
	},
	"voxels" : [ //X
		[		// Y
			[{
				"materials" : {
					"top" : "MATERIAL_ID",
					"bottom": "MATERIAL_ID",
					"front" : "MATERIAL_ID",
					"back" : "MATERIAL_ID",
					"left" : "MATERIAL_ID",
					"right" : "MATERIAL_ID"
				},
				/* optional */
				"custom_mesh" : CustomMeshObject /* To be implemented */
			}] //Z
		]
	]

}
```
